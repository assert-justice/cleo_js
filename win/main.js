import{setInit,setUpdate,println} from 'cleo';
import {setClearColor} from 'cleo-renderer'; 
println('yo');
setInit(()=>{
    println('sup');
    setClearColor(0,0,1);
});
setUpdate((dt)=>{println('rad' + dt);});
// setClearColor(0,0,1);
