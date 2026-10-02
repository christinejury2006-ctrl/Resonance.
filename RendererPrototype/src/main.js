import * as pc from 'playcanvas';
import './style.css';

const canvas = document.querySelector('#application');
const status = document.querySelector('#status');
const errorBox = document.querySelector('#error');

const DRAGON_URL = '/api/dragon';

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

  const app = new pc.Application(canvas, { graphicsDevice: device });
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

  // The source GLB is authored with fully metallic PBR values for several
  // organic surfaces. That makes broad faces catch hard specular highlights
  // and exaggerates the low-poly silhouette. Keep the authored textures and
  // normal maps, but use physically plausible non-metallic response.
  dragon.findComponents('render').forEach((render) => {
    render.meshInstances.forEach((meshInstance) => {
      const material = meshInstance.material;
      if (!material) return;

      const name = (material.name || '').toLowerCase();
      // Never allow the imported material to force flat/triangle shading.
      // PlayCanvas uses flat shading for GLB primitives that arrive without
      // usable vertex normals; we repair those normals below from the mesh
      // positions and triangle indices.
      material.flatShading = false;
      material.useMetalness = true;

      if (name.includes('dragon_scales')) {
        material.metalness = 0.08;
        material.roughness = 0.72;
        material.shininess = 18;
      } else if (name.includes('wing_membrane')) {
        material.metalness = 0.02;
        material.roughness = 0.58;
        material.shininess = 24;
      } else if (name.includes('bone_horn')) {
        material.metalness = 0.0;
        material.roughness = 0.46;
        material.shininess = 30;
      }

      material.update();
    });
  });
  // Repair missing/faceted vertex normals while preserving the GLB's
  // positions, UVs, skin weights and indices. This changes shading, not the
  // actual silhouette, so genuinely low-resolution geometry remains visible.
  let repairedMeshes = 0;
  let normalSamples = 0;
  dragon.findComponents('render').forEach((render) => {
    render.meshInstances.forEach((meshInstance) => {
      const mesh = meshInstance.mesh;
      if (!mesh) return;

      const positions = new Float32Array(mesh.vertexBuffer.numVertices * 3);
      const indices = mesh.indexBuffer
        ? (() => {
            const count = mesh.primitive[0].count;
            const values = new Uint32Array(count);
            mesh.getIndices(values);
            return values;
          })()
        : null;

      if (!indices) return;
      mesh.getPositions(positions);

      const normals = pc.calculateNormals(positions, indices);
      if (normals && normals.length === positions.length) {
        mesh.setNormals(normals);
        mesh.update(pc.PRIMITIVE_TRIANGLES, false);
        repairedMeshes += 1;
        normalSamples += normals.length / 3;
      }
    });
  });

  // Play the authored flap animation, then add subtle secondary finger motion.
  // The source Flap clip drives the three main wing bones but leaves the finger
  // bones rigid. We layer a small, phase-shifted response onto those fingers
  // so the membrane can flex instead of behaving like a flat sheet.
  dragon.addComponent('anim', { activate: false });
  const animationTracks = asset.resource.animations || [];
  for (const track of animationTracks) {
    if (track?.resource) {
      dragon.anim.assignAnimation(track.name, track.resource);
    }
  }
  if (dragon.anim.baseLayer && animationTracks.length) {
    dragon.anim.baseLayer.transition('Flap');
  }

  const wingFingers = [];
  dragon.findByName('finger1_L_1') && wingFingers.push(dragon.findByName('finger1_L_1'));
  dragon.findByName('finger1_L_2') && wingFingers.push(dragon.findByName('finger1_L_2'));
  dragon.findByName('finger2_L_1') && wingFingers.push(dragon.findByName('finger2_L_1'));
  dragon.findByName('finger2_L_2') && wingFingers.push(dragon.findByName('finger2_L_2'));
  dragon.findByName('finger3_L_1') && wingFingers.push(dragon.findByName('finger3_L_1'));
  dragon.findByName('finger3_L_2') && wingFingers.push(dragon.findByName('finger3_L_2'));
  dragon.findByName('finger4_L_1') && wingFingers.push(dragon.findByName('finger4_L_1'));
  dragon.findByName('finger4_L_2') && wingFingers.push(dragon.findByName('finger4_L_2'));
  dragon.findByName('finger1_R_1') && wingFingers.push(dragon.findByName('finger1_R_1'));
  dragon.findByName('finger1_R_2') && wingFingers.push(dragon.findByName('finger1_R_2'));
  dragon.findByName('finger2_R_1') && wingFingers.push(dragon.findByName('finger2_R_1'));
  dragon.findByName('finger2_R_2') && wingFingers.push(dragon.findByName('finger2_R_2'));
  dragon.findByName('finger3_R_1') && wingFingers.push(dragon.findByName('finger3_R_1'));
  dragon.findByName('finger3_R_2') && wingFingers.push(dragon.findByName('finger3_R_2'));
  dragon.findByName('finger4_R_1') && wingFingers.push(dragon.findByName('finger4_R_1'));
  dragon.findByName('finger4_R_2') && wingFingers.push(dragon.findByName('finger4_R_2'));

  dragon.name = 'Dragon_Cinematic';
  dragon.setPosition(0, 0, 0);
  app.root.addChild(dragon);

  // Touch-first inspection controls: drag to orbit, pinch/wheel to zoom.
  let yaw = 0;
  let pitch = 0.08;
  let distance = 7.2;
  let dragging = false;
  let lastX = 0;
  let lastY = 0;
  let pinchStart = null;

  const target = new pc.Vec3(0, 1.45, 0);
  const updateCamera = () => {
    const cp = Math.cos(pitch);
    camera.setPosition(
      target.x + Math.sin(yaw) * cp * distance,
      target.y + Math.sin(pitch) * distance,
      target.z + Math.cos(yaw) * cp * distance
    );
    camera.lookAt(target);
  };

  canvas.addEventListener('pointerdown', (event) => {
    dragging = true;
    lastX = event.clientX;
    lastY = event.clientY;
    canvas.setPointerCapture(event.pointerId);
  });

  canvas.addEventListener('pointermove', (event) => {
    if (!dragging) return;
    const dx = event.clientX - lastX;
    const dy = event.clientY - lastY;
    lastX = event.clientX;
    lastY = event.clientY;
    yaw -= dx * 0.008;
    pitch = Math.max(-0.35, Math.min(0.45, pitch + dy * 0.005));
    updateCamera();
  });

  canvas.addEventListener('pointerup', (event) => {
    dragging = false;
    canvas.releasePointerCapture(event.pointerId);
  });

  canvas.addEventListener('pointercancel', () => { dragging = false; });
  canvas.addEventListener('wheel', (event) => {
    event.preventDefault();
    distance = Math.max(3.2, Math.min(12, distance + event.deltaY * 0.008));
    updateCamera();
  }, { passive: false });

  canvas.addEventListener('touchstart', (event) => {
    if (event.touches.length === 2) {
      const a = event.touches[0];
      const b = event.touches[1];
      pinchStart = Math.hypot(a.clientX - b.clientX, a.clientY - b.clientY);
    }
  }, { passive: true });

  canvas.addEventListener('touchmove', (event) => {
    if (event.touches.length !== 2 || pinchStart === null) return;
    event.preventDefault();
    const a = event.touches[0];
    const b = event.touches[1];
    const current = Math.hypot(a.clientX - b.clientX, a.clientY - b.clientY);
    distance = Math.max(3.2, Math.min(12, distance * (pinchStart / Math.max(current, 1))));
    pinchStart = current;
    updateCamera();
  }, { passive: false });

  canvas.addEventListener('touchend', () => { pinchStart = null; });

  setStatus(`Dragon loaded • smooth shading • ${repairedMeshes} meshes / ${Math.round(normalSamples).toLocaleString()} vertices`);
  app.start();

  updateCamera();
  app.on('update', (dt) => {\n    updateCamera();\n    if (dragon.anim?.baseLayer?.activeState === 'Flap') {\n      const t = dragon.anim.baseLayer.activeStateCurrentTime;\n      wingFingers.forEach((bone, index) => {\n        const side = bone.name.includes('_R_') ? -1 : 1;\n        const segment = bone.name.endsWith('_2') ? 1 : 0;\n        const finger = Number(bone.name.match(/finger(\\d)/)?.[1] || 1);\n        const phase = t * 10.5 + finger * 0.32 + segment * 0.22;\n        const flex = Math.sin(phase) * (0.055 + finger * 0.012);\n        bone.setLocalEulerAngles(bone.getLocalEulerAngles().x, bone.getLocalEulerAngles().y, side * flex * 57.3);\n      });\n    }\n  });
}

boot().catch(fail);
