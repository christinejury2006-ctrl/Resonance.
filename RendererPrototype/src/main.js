import * as pc from 'playcanvas';
import './style.css';

const canvas = document.querySelector('#application');
const status = document.querySelector('#status');
const errorBox = document.querySelector('#error');

const DRAGON_URL = 'https://github.com/christinejury2006-ctrl/Resonance./raw/prototype/playcanvas-cinematic/Content/External/Dragon/war_dragon_rigged.glb';

function setStatus(message) {
  status.textContent = message.toUpperCase();
}

function fail(error) {
  const message = error && error.message ? error.message : String(error);
  console.error(error);
  errorBox.textContent = 'Could not start the Dragonbound renderer. ' + message;
  errorBox.hidden = false;
  setStatus('Renderer error');
}

async function boot() {
  setStatus('Creating WebGPU renderer');

  const device = await pc.createGraphicsDevice(canvas, {
    deviceTypes: [pc.DEVICETYPE_WEBGPU, pc.DEVICETYPE_WEBGL2],
    antialias: true,
    powerPreference: 'high-performance'
  });

  const options = new pc.AppOptions();
  options.graphicsDevice = device;
  options.componentSystems = [
    pc.RenderComponentSystem,
    pc.CameraComponentSystem,
    pc.LightComponentSystem
  ];
  options.resourceHandlers = [
    pc.TextureHandler,
    pc.ContainerHandler
  ];

  const app = new pc.AppBase(canvas);
  app.init(options);
  app.setCanvasFillMode(pc.FILLMODE_FILL_WINDOW);
  app.setCanvasResolution(pc.RESOLUTION_AUTO);

  app.scene.exposure = 1.0;
  app.scene.ambientLight = new pc.Color(0.035, 0.045, 0.06);

  const camera = new pc.Entity('CinematicCamera');
  camera.addComponent('camera', {
    clearColor: new pc.Color(0.006, 0.008, 0.012),
    fov: 42,
    nearClip: 0.05,
    farClip: 5000
  });
  camera.camera.toneMapping = pc.TONEMAP_ACES;
  camera.setPosition(0, 1.8, 7.2);
  camera.lookAt(0, 1.45, 0);
  app.root.addChild(camera);

  const key = new pc.Entity('KeyLight');
  key.addComponent('light', {
    type: 'directional',
    color: new pc.Color(1.0, 0.74, 0.52),
    intensity: 2.7,
    castShadows: true,
    shadowResolution: 2048,
    shadowDistance: 35,
    normalOffsetBias: 0.04
  });
  key.setEulerAngles(38, -42, 0);
  app.root.addChild(key);

  const fill = new pc.Entity('CoolFill');
  fill.addComponent('light', {
    type: 'omni',
    color: new pc.Color(0.22, 0.38, 0.75),
    intensity: 8,
    range: 12
  });
  fill.setPosition(-4, 2.5, 2);
  app.root.addChild(fill);

  const rim = new pc.Entity('WarmRim');
  rim.addComponent('light', {
    type: 'spot',
    color: new pc.Color(1.0, 0.3, 0.12),
    intensity: 16,
    range: 18,
    innerConeAngle: 18,
    outerConeAngle: 35,
    castShadows: true,
    shadowResolution: 1024
  });
  rim.setPosition(3.8, 3.8, -3.5);
  rim.lookAt(0, 1.5, 0);
  app.root.addChild(rim);

  const ground = new pc.Entity('Ground');
  ground.addComponent('render', {
    type: 'plane',
    receiveShadows: true
  });
  ground.setLocalScale(16, 1, 16);

  const groundMaterial = new pc.StandardMaterial();
  groundMaterial.diffuse = new pc.Color(0.018, 0.021, 0.025);
  groundMaterial.roughness = 0.92;
  groundMaterial.metalness = 0.02;
  groundMaterial.update();
  ground.render.material = groundMaterial;
  app.root.addChild(ground);

  setStatus('Loading dragon');

  const asset = await new Promise((resolve, reject) => {
    app.assets.loadFromUrl(DRAGON_URL, 'container', (err, loaded) => {
      if (err) reject(err);
      else resolve(loaded);
    });
  });

  const dragon = asset.resource.instantiateRenderEntity();
  dragon.name = 'Dragon_Cinematic';
  dragon.setPosition(0, 0, 0);
  app.root.addChild(dragon);

  setStatus('Dragon loaded • cinematic lighting active');
  app.start();

  let time = 0;
  app.on('update', (dt) => {
    time += dt;
    camera.setPosition(
      Math.sin(time * 0.12) * 0.22,
      1.8 + Math.sin(time * 0.17) * 0.035,
      7.2 + Math.cos(time * 0.12) * 0.18
    );
    camera.lookAt(0, 1.45, 0);
  });
}

boot().catch(fail);
