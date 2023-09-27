import {Game, System, Input, Window} from 'cleo';

Game.init = ()=>{
    System.println('sup');
    System.println(''+Window.width);
    System.println(''+Window.height);
}
// Game.init = ()=>println("sup: "+System.println);

Game.update = ()=>{
    if(Input.getKey(256)) Game.quit();
}