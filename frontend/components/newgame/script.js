function intNewGame(){
const apps=await window.api.listApps();
const pid=apps.data.pids[0];

const bytes=new Uint8Array([0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00]);
await window.api.writeApp(pid,bytes); 
}

intNewGame();