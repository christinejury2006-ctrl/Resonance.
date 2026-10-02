import * as pc from 'playcanvas';
import './style.css';

const canvas = document.querySelector('#application');
const status = document.querySelector('#status');
const errorBox = document.querySelector('#error');
const resetButton = document.querySelector('#reset');
const idleButton = document.querySelector('#idle');
const DRAGON_URL = '/api/dragon';

function setStatus(message){ status.textContent = message.toUpperCase(); }
function fail(error){
  console.error(error);
  errorBox.textContent = 'Could not start the Resonance renderer. ' + (error?.message || error);
  errorBox.hidden = false;
  setStatus('Renderer error');
}

async function boot(){
  setStatus('Creating graphics renderer');
  const device = await pc.createGraphicsDevice(canvas,{
    deviceTypes:[pc.DEVICETYPE_WEBGPU,pc.DEVICETYPE_WEBGL2],
    antialias:true,
    powerPreference:'high-performance'
  });

  const options = new pc.AppOptions();
  options.graphicsDevice = device;
  options.componentSystems = [pc.RenderComponentSystem,pc.CameraComponentSystem,pc.LightComponentSystem];
  options.resourceHandlers = [pc.TextureHandler,pc.ContainerHandler];

  const app = new pc.AppBase(canvas);
  app.init(options);
  app.setCanvasFillMode(pc.FILLMODE_FILL_WINDOW);
  app.setCanvasResolution(pc.RESOLUTION_AUTO);
  app.scene.exposure = 1;
  app.scene.ambientLight = new pc.Color(.035,.045,.06);

  const camera = new pc.Entity('CinematicCamera');
  camera.addComponent('camera',{clearColor:new pc.Color(.006,.008,.012),fov:42,nearClip:.05,farClip:5000});
  camera.camera.toneMapping = pc.TONEMAP_ACES;
  app.root.addChild(camera);

  const key = new pc.Entity('KeyLight');
  key.addComponent('light',{type:'directional',color:new pc.Color(1,.74,.52),intensity:2.7,castShadows:true,shadowResolution:2048,shadowDistance:35,normalOffsetBias:.04});
  key.setEulerAngles(38,-42,0); app.root.addChild(key);

  const fill = new pc.Entity('CoolFill');
  fill.addComponent('light',{type:'omni',color:new pc.Color(.22,.38,.75),intensity:8,range:12});
  fill.setPosition(-4,2.5,2); app.root.addChild(fill);

  const rim = new pc.Entity('WarmRim');
  rim.addComponent('light',{type:'spot',color:new pc.Color(1,.3,.12),intensity:16,range:18,innerConeAngle:18,outerConeAngle:35,castShadows:true,shadowResolution:1024});
  rim.setPosition(3.8,3.8,-3.5); rim.lookAt(0,1.5,0); app.root.addChild(rim);

  const ground = new pc.Entity('Ground');
  ground.addComponent('render',{type:'plane',receiveShadows:true});
  ground.setLocalScale(16,1,16);
  const gm = new pc.StandardMaterial();
  gm.diffuse = new pc.Color(.018,.021,.025); gm.roughness=.92; gm.metalness=.02; gm.update();
  ground.render.material=gm; app.root.addChild(ground);

  setStatus('Loading Resonance');
  const asset = await new Promise((resolve,reject)=>{
    app.assets.loadFromUrl(DRAGON_URL,'container',(err,loaded)=>err?reject(err):resolve(loaded));
  });
  const dragon = asset.resource.instantiateRenderEntity();
  dragon.name='Resonance_Trellis_Dragon';

  let repairedMeshes=0;
  dragon.findComponents('render').forEach(render=>{
    render.meshInstances.forEach(mi=>{
      const material=mi.material;
      if(material){
        material.flatShading=false;
        material.useMetalness=true;
        const name=(material.name||'').toLowerCase();
        if(name.includes('scale')){material.metalness=.08;material.roughness=.72;}
        else if(name.includes('membrane')||name.includes('wing')){material.metalness=.02;material.roughness=.58;}
        else {material.metalness=Math.min(material.metalness??0,.15);material.roughness=Math.max(material.roughness??.5,.48);}
        material.update();
      }
      const mesh=mi.mesh;
      if(!mesh||!mesh.indexBuffer)return;
      try{
        const positions=new Float32Array(mesh.vertexBuffer.numVertices*3);
        const count=mesh.primitive[0].count;
        const indices=new Uint32Array(count);
        mesh.getIndices(indices);
        mesh.getPositions(positions);
        const normals=pc.calculateNormals(positions,indices);
        if(normals&&normals.length===positions.length){
          mesh.setNormals(normals); mesh.update(pc.PRIMITIVE_TRIANGLES,false); repairedMeshes++;
        }
      }catch(e){ console.warn('Normal repair skipped',e); }
    });
  });

  app.root.addChild(dragon);

  // TRELLIS exports can use arbitrary world units/origins. Normalize the asset
  // from its actual render bounds so it is grounded, centered, and fills the
  // viewport instead of appearing tiny or below the camera.
  let minX=Infinity,minY=Infinity,minZ=Infinity,maxX=-Infinity,maxY=-Infinity,maxZ=-Infinity;
  dragon.findComponents('render').forEach(render=>{
    render.meshInstances.forEach(mi=>{
      const box=mi.aabb;
      if(!box)return;
      const c=box.center,e=box.halfExtents;
      minX=Math.min(minX,c.x-e.x); maxX=Math.max(maxX,c.x+e.x);
      minY=Math.min(minY,c.y-e.y); maxY=Math.max(maxY,c.y+e.y);
      minZ=Math.min(minZ,c.z-e.z); maxZ=Math.max(maxZ,c.z+e.z);
    });
  });
  const rawHeight=Math.max(maxY-minY,0.001);
  const modelScale=3.35/rawHeight;
  dragon.setLocalScale(modelScale,modelScale,modelScale);
  const centerX=(minX+maxX)*0.5, centerZ=(minZ+maxZ)*0.5;
  dragon.setLocalPosition(-centerX*modelScale,-minY*modelScale,-centerZ*modelScale);

  let yaw=.05,pitch=.08,distance=6,dragging=false,lastX=0,lastY=0,pinchStart=null,paused=false;
  const target=new pc.Vec3(0,1.62,0);
  const updateCamera=()=>{
    const cp=Math.cos(pitch);
    camera.setPosition(target.x+Math.sin(yaw)*cp*distance,target.y+Math.sin(pitch)*distance,target.z+Math.cos(yaw)*cp*distance);
    camera.lookAt(target);
  };
  updateCamera();

  canvas.addEventListener('pointerdown',e=>{dragging=true;lastX=e.clientX;lastY=e.clientY;canvas.setPointerCapture(e.pointerId)});
  canvas.addEventListener('pointermove',e=>{
    if(!dragging)return;
    yaw-=(e.clientX-lastX)*.008; pitch=Math.max(-.35,Math.min(.45,pitch+(e.clientY-lastY)*.005));
    lastX=e.clientX;lastY=e.clientY;updateCamera();
  });
  canvas.addEventListener('pointerup',e=>{dragging=false;try{canvas.releasePointerCapture(e.pointerId)}catch{}});
  canvas.addEventListener('pointercancel',()=>dragging=false);
  canvas.addEventListener('wheel',e=>{e.preventDefault();distance=Math.max(3.2,Math.min(12,distance+e.deltaY*.008));updateCamera()},{passive:false});
  canvas.addEventListener('touchstart',e=>{
    if(e.touches.length===2){const a=e.touches[0],b=e.touches[1];pinchStart=Math.hypot(a.clientX-b.clientX,a.clientY-b.clientY)}
  },{passive:true});
  canvas.addEventListener('touchmove',e=>{
    if(e.touches.length!==2||pinchStart===null)return;
    e.preventDefault();const a=e.touches[0],b=e.touches[1];const d=Math.hypot(a.clientX-b.clientX,a.clientY-b.clientY);
    distance=Math.max(3.2,Math.min(12,distance*(pinchStart/Math.max(d,1))));pinchStart=d;updateCamera();
  },{passive:false});
  canvas.addEventListener('touchend',()=>pinchStart=null);

  resetButton.addEventListener('click',()=>{yaw=.05;pitch=.08;distance=6;updateCamera();});
  idleButton.addEventListener('click',()=>{paused=!paused;idleButton.textContent=paused?'RESUME IDLE':'PAUSE IDLE';});

  // The TRELLIS Resonance.glb is mesh-only: there is no skeleton/animation clip.
  // Do not fake a body-bob and present it as a wing animation. If TRELLIS exported
  // the wings as separate scene nodes, flap those nodes procedurally; otherwise
  // leave the creature visually still until a properly rigged animation asset exists.
  const wingNodes=[];
  dragon.find(node=>{
    const n=(node.name||'').toLowerCase();
    if((n.includes('wing')||n.includes('leftwing')||n.includes('rightwing')) && node!==dragon){
      wingNodes.push({node,base:node.getLocalEulerAngles().clone()});
    }
    return false;
  });

  setStatus(wingNodes.length ? 'Resonance loaded · wing animation' : 'Resonance loaded · mesh preview');
  app.start();
  app.on('update',dt=>{
    if(paused)return;
    if(!wingNodes.length)return;
    const t=performance.now()*.001;
    const flap=Math.sin(t*2.2)*11;
    wingNodes.forEach(({node,base},i)=>{
      const direction=i%2===0?1:-1;
      node.setLocalEulerAngles(base.x,base.y,base.z+flap*direction);
    });
  });
  console.info('Resonance renderer ready',{repairedMeshes,wingNodes:wingNodes.length});
}

boot().catch(fail);
