#include "cmd.h"
#include "mem.h"
#include "io.h"
#include "bda.h"

/* ── helpers ── */


/* ── commands ── */

//no args
void session_new(void) {

    uint64_t temp_ptr_size = 8*1024;
    aligned_free(temp_ptr);
    temp_ptr = aligned_calloc(temp_ptr_size, 64);

    *(uint64_t *)(base_ptr+8) = lv_count*(sizeof(logical_volume_header));

    lv = ( logical_volume_header *)(base_ptr+16);

    //LV1 header, global gamestate
    lv[0].size = sizeof(game_state_structure);
    lv[0].ptr = 16+*(uint64_t *)(base_ptr+8)+63;
    lv[0].ptr -= lv[0].ptr%64;
    lv[0].id = 1;

    //LV1 contents, global gamestate

    game_state = (game_state_structure *)(base_ptr+lv[0].ptr);

    read_file_from_exe_dir("data/gamestate.bda", temp_ptr, temp_ptr_size);
    bda_descriptor bda;
    uint64_t *bda_dest = (uint64_t *)(temp_ptr+(temp_ptr_size/2));
    bda.n = 7;
    bda.x = 7;
    bda.y = 1;
    bda.src = (uint64_t)temp_ptr;
    bda.dest = (uint64_t)bda_dest;
    bda_parse(&bda);

    (*game_state).money = *bda_dest;
    (*game_state).view_1.x = (int32_t)*(bda_dest+1);
    (*game_state).view_1.y = (int32_t)*(bda_dest+2);
    (*game_state).view_2.x = (int32_t)*(bda_dest+3);
    (*game_state).view_2.y = (int32_t)*(bda_dest+4);
    (*game_state).map.x = (int32_t)*(bda_dest+5);
    (*game_state).map.y = (int32_t)*(bda_dest+6);


    //LV2 header, building sizes

    lv[1].size=sizeof(building_size)*building_count;
    lv[1].ptr = lv[0].ptr+lv[0].size+63;
    lv[1].ptr -= lv[1].ptr%64;
    lv[1].id = 2;
    
    //LV2 contents, building sizes

    memset(temp_ptr, 0, temp_ptr_size);
    read_file_from_exe_dir("data/buildings/sizes.bda", temp_ptr, temp_ptr_size);
    bda.n = building_count*2;
    bda.x = 2;
    bda.y = building_count;
    bda.src = (uint64_t)temp_ptr;
    bda_parse(&bda);

    for(struct { building_size *ptr; int32_t i;} s = { (building_size *)(base_ptr+lv[1].ptr), 0}; s.i<building_count; s.i++){
        s.ptr[s.i].x = (uint8_t)bda_dest[2*s.i];
        s.ptr[s.i].y = (uint8_t)bda_dest[2*s.i+1];
    }

    //LV3 header, building prices

    lv[2].size=sizeof(double)*building_count;
    lv[2].ptr = lv[1].ptr+lv[1].size+63;
    lv[2].ptr -= lv[2].ptr%64;
    lv[2].id = 3;

    //LV3 contents, building prices

    memset(temp_ptr, 0, temp_ptr_size);
    read_file_from_exe_dir("data/buildings/prices.bda", temp_ptr, temp_ptr_size);
    bda.n = building_count;
    bda.x = building_count;
    bda.y = 1;
    bda.src = (uint64_t)temp_ptr;
    bda_parse(&bda);

    for(struct { double *ptr; int32_t i;} s = { (double *)(base_ptr+lv[2].ptr), 0}; s.i<building_count; s.i++){
        s.ptr[s.i] = (double)bda_dest[s.i];
    }

    //LV4 header, building actions

    lv[3].size=sizeof(building_action)*building_count;
    lv[3].ptr = lv[2].ptr+lv[2].size+63;
    lv[3].ptr -= lv[3].ptr%64;
    lv[3].id = 4;

    //LV4 contents, building actions

    //to be added

    //LV5 header, marine building sizes

    lv[4].size=sizeof(building_size)*marine_building_count;
    lv[4].ptr = lv[3].ptr+lv[3].size+63;
    lv[4].ptr -= lv[4].ptr%64;
    lv[4].id = 5;

    //LV5 contents, marine building sizes

    memset(temp_ptr, 0, temp_ptr_size);
    read_file_from_exe_dir("data/buildings/marine/sizes.bda", temp_ptr, temp_ptr_size);
    bda.n = marine_building_count*2;
    bda.x = 2;
    bda.y = marine_building_count;
    bda.src = (uint64_t)temp_ptr;
    bda_parse(&bda);

    for(struct { building_size *ptr; int32_t i;} s = { (building_size *)(base_ptr+lv[4].ptr), 0}; s.i<marine_building_count; s.i++){
        s.ptr[s.i].x = (uint8_t)bda_dest[2*s.i];
        s.ptr[s.i].y = (uint8_t)bda_dest[2*s.i+1];
    }

    //LV6 header, marine building prices

    lv[5].size=sizeof(double)*marine_building_count;
    lv[5].ptr = lv[4].ptr+lv[4].size+63;
    lv[5].ptr -= lv[5].ptr%64;
    lv[5].id = 6;

    //LV6 contents, marine building prices

    memset(temp_ptr, 0, temp_ptr_size);
    read_file_from_exe_dir("data/buildings/marine/prices.bda", temp_ptr, temp_ptr_size);
    bda.n = marine_building_count;
    bda.x = marine_building_count;
    bda.y = 1;
    bda.src = (uint64_t)temp_ptr;
    bda_parse(&bda);

    for(struct { double *ptr; int32_t i;} s = { (double *)(base_ptr+lv[5].ptr), 0}; s.i<marine_building_count; s.i++){
        s.ptr[s.i] = (double)bda_dest[s.i];
    }

    //LV7 header, marine building actions

    lv[6].size=sizeof(building_action)*marine_building_count;
    lv[6].ptr = lv[5].ptr+lv[5].size+63;
    lv[6].ptr -= lv[6].ptr%64;
    lv[6].id = 7;

    //LV7 contents, marine building actions

    //to be added

    //LV8 header, tiles
    lv[7].size = sizeof(int32_t)*(*game_state).map.x*(*game_state).map.y;
    lv[7].ptr = lv[6].ptr+lv[6].size+63;
    lv[7].ptr -= lv[7].ptr%64;
    lv[7].id = 8;

    //LV8 contents, tiles
    int32_t *tile_ptr = (int32_t *)(base_ptr + lv[7].ptr);

    for (uint64_t y = 0; y < (*game_state).map.y; y++) {
        for (uint64_t x = 0; x < (*game_state).map.x; x++) {
            int32_t val = 0;

            if (y < 4) {
                val = -1;
            } else if (x == 2) {
                val = 1;
            }

            *tile_ptr = val;
            tile_ptr++;
        }
    }

    return;
}

//string path, null terminated
void session_load(void) {
    return;
}

//4 byte ms to define tick
void tick_set(void) {
    uint32_t ms;
    if (fread(&ms, 4, 1, stdin) == 1)
        time_ms = ms;
    return;
}

//no args
void dump_to_file(void){
    char exe_path[1024];
    char full_path[2048];


    if (get_exe_directory(exe_path, sizeof(exe_path)) == 0) {

        snprintf(full_path, sizeof(full_path), "%s/dump.bin", exe_path);
    } else {

        strcpy(full_path, "dump.bin");
    }

    FILE *f = fopen(full_path, "wb");
    if (f) {
        fwrite(base_ptr, 1, *(uint64_t *)base_ptr, f);
        fclose(f);
    } else {
        printf("DEBUG: Failed to open %s. Reason: %s\n", full_path, strerror(errno));
    }
    return;
}

//4 byte x, 4 byte y, 4 byte building ID
void tile_build (void){
    int32_t id, real_id;
    uint32_t x, y;
    fread(&x, 4, 1, stdin);
    fread(&y, 4, 1, stdin);
    fread(&id, 4, 1, stdin);
    real_id = id;

    if(x<0 || y<0 || x>(*game_state).map.x || y>(*game_state).map.y){
        //error
    }


    uint8_t *ptr;
    double price;
    if(id>=0){
        ptr = base_ptr+lv[1].ptr;
        price = *(double*)(base_ptr+lv[2].ptr+id*8);
    }else{
        ptr = base_ptr+lv[4].ptr;
        id*=(-1);
        id-=1;
         price = *(double*)(base_ptr+lv[5].ptr+id*8);
    }
    if(price>(*game_state).money)
    {
        //error
    } else{
        (*game_state).money -= price;
    }

    uint8_t size_x, size_y;

    uint8_t *size_ptr=ptr+id*2;
    size_x = *size_ptr;
    size_y = *(size_ptr+1);

    uint32_t new_x, new_y;

    new_x = x+1-size_x;
    new_y = y-1+size_y;

    if(new_x<0 || new_y<0 || new_x>(*game_state).map.x || new_y>(*game_state).map.y){
        //error
    }
    
    int32_t shadow_id=0;
    if(size_x !=1 || size_y != 1){
        while(*size_ptr!=0 || *(size_ptr+1) !=0){
            size_ptr+=2;
        }
        uint64_t shadow;
        shadow = (uint64_t)size_ptr-(uint64_t)base_ptr;
        if(real_id >= 0){
            shadow-=lv[1].ptr;
        } else{
            shadow-=lv[4].ptr;
        }
        shadow_id=shaodw/2;
        if(real_id<0){
            shadow_id*=(-1);
            shaodw_id-=1;
        }
    }

    //todo, construct building on map

    //uint64_t ptr=lv[3].ptr;
    //*(int32_t *)((int32_t *)ptr+y*(*game_state).map.x+x)=id;

    return;
}

//4 byte x, 4 byte y
void tile_clear (void){
    uint32_t x, y;
    fread(&x, 4, 1, stdin);
    fread(&y, 4, 1, stdin);
    uint64_t ptr=lv[3].ptr;
    *(int32_t *)((int32_t *)ptr+y*(*game_state).map.x+x)=0;

    return;
}

// 16 bytes: x_1, y_1, x_2, y_2
void viewbox_register (void) {
    int32_t x_1, y_1, x_2, y_2;
    fread(&x_1, 4, 1, stdin);
    fread(&y_1, 4, 1, stdin);
    fread(&x_2, 4, 1, stdin);
    fread(&y_2, 4, 1, stdin);
    (*game_state).view_1.x = x_1;
    (*game_state).view_1.y = y_1;
    (*game_state).view_2.x = x_2;
    (*game_state).view_2.y = y_2;
    return;

}   
