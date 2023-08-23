// import * as my from 'MyModule';
import {println, setInit} from 'MyModule';
println("hi");
setInit(()=>{println("meep")});
