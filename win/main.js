import{setInit,setUpdate,println} from 'cleo';
import {setClearColor, loadImage, drawImage} from 'cleo-renderer'; 
// println('yo');
let spriteSheet = 0;
setInit(()=>{
    setClearColor(0,0,1);
    spriteSheet = loadImage('./characters_packed.png');
    println(''+spriteSheet);
});
setUpdate((dt)=>{
    // println('sup');
    drawImage(spriteSheet, 0, 0, 24*4, 24*4, 0, 0, 24, 24);
    drawImage(spriteSheet, 24*4, 0, 24*4, 24*4, 24*2, 0, 24, 24);
});
