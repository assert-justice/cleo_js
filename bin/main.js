import {Game, System, Input} from 'cleo';

Game.init = ()=>System.println("sup");
// Game.init = ()=>println("sup: "+System.println);

Game.update = ()=>{
    if(Input.getKey(256)) System.println("yo");
}