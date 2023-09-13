import{setInit,setUpdate,println} from 'cleo';
import {setClearColor, loadImage, drawImage} from 'cleo-renderer';
import {getKey} from 'cleo-input';
// println('yo');
let spriteSheet = 0;
const player = {
    x: 0,
    y: 0,
    speed: 100,
};
setInit(()=>{
    setClearColor(0,0,1);
    spriteSheet = loadImage('./characters_packed.png');
    // println(''+spriteSheet);
});
setUpdate((dt)=>{
    // println('sup');
    if(getKey(262)) player.x += player.speed * dt;
    if(getKey(263)) player.x -= player.speed * dt;
    drawImage(spriteSheet, player.x, player.y, 24*4, 24*4, 0, 0, 24, 24);
    // drawImage(spriteSheet, 24*4, 0, 24*4, 24*4, 24*2, 0, 24, 24);
});
