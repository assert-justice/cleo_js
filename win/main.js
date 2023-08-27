import{setInit,setUpdate,println} from 'cleo';
import {setClearColor} from 'cleo-renderer'; 
println('yo');
setInit(()=>{
    println('sup');
    setClearColor(0,0,1);
});
setUpdate(()=>{println('rad');});
// setClearColor(0,0,1);
