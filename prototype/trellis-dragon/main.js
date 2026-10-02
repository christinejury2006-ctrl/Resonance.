const canvas = document.getElementById("application-canvas");
const status = document.getElementById("status");
const loading = document.getElementById("loading");
const toast = document.getElementById("toast");
const resetButton = document.getElementById("reset");
const pauseButton = document.getElementById("pause");

const app = new pc.Application(canvas, {
  graphicsDeviceOptions: { antialias: true, alpha: false, preserveDrawingBuffer: false }
});
app.setCanvasFillMode(pc.FILLMODE_FILL_WINDOW);
app.setCanvasResolution(pc.RESOLUTION_AUTO);
app.scene.ambientLight = new pc.Color(0.13, 0.15, 0.22);

const camera = new pc.Entity("Camera");
camera.addComponent("camera", {
  clearColor: new pc.Color(0.025, 0.03, 0.055),
  fov: 42,
  nearClip: 0.05,
  farClip: 100
});
app.root.addChild(camera);

const key = new pc.Entity("KeyLight");
key.addComponent("light", {
  type: "directional",
  color: new pc.Color(1, 0.88, 0.74),
  intensity: 3.2,
  castShadows: true,
  shadowResolution: 1024,
  shadowDistance: 30
});
key.setEulerAngles(-42, 32, 0);
app.root.addChild(key);

const fill = new pc.Entity("FillLight");
fill.addComponent("light", {
  type: "directional",
  color: new pc.Color(0.45, 0.6, 1),
  intensity: 1.4
});
fill.setEulerAngles(18, -140, 0);
app.root.addChild(fill);

const rim = new pc.Entity("RimLight");
rim.addComponent("light", {
  type: "point",
  color: new pc.Color(0.35, 0.55, 1),
  intensity: 7,
  range: 12
});
rim.setPosition(-3, 3, -4);
app.root.addChild(rim);

const ground = new pc.Entity("Ground");
ground.addComponent("render", { type: "plane" });
ground.setLocalScale(18, 1, 18);
ground.setPosition(0, -1.35, 0);
ground.render.castShadows = false;
ground.render.receiveShadows = true;
const groundMaterial = new pc.StandardMaterial();
groundMaterial.diffuse = new pc.Color(0.035, 0.042, 0.065);
groundMaterial.metalness = 0.45;
groundMaterial.gloss = 0.72;
groundMaterial.update();
ground.render.material = groundMaterial;
app.root.addChild(ground);

const dragonPivot = new pc.Entity("ResonancePivot");
app.root.addChild(dragonPivot);

const assetUrl = "https://github.com/christinejury2006-ctrl/Resonance./releases/download/renderer-assets/Resonance.glb";
const dragonAsset = new pc.Asset("Resonance", "container", { url: assetUrl });

let dragon = null;
let paused = false;
let dragging = false;
let lastX = 0;
let lastY = 0;
let yaw = 18;
let pitch = -7;
let distance = 7.2;
let baseY = -0.65;
let toastTimer;

function say(message) {
  toast.textContent = message;
  toast.classList.add("show");
  clearTimeout(toastTimer);
  toastTimer = setTimeout(() => toast.classList.remove("show"), 1600);
}

function frameDragon() {
  if (!dragon) return;
  const half = dragon.render?.aabb?.halfExtents;
  const center = dragon.render?.aabb?.center;
  if (half && center) {
    const radius = Math.max(half.x, half.y, half.z) * 2.25;
    distance = Math.max(4.5, Math.min(12, radius));
    baseY = -center.y + 0.15;
  }
}

function updateCamera() {
  const yawRad = yaw * Math.PI / 180;
  const pitchRad = pitch * Math.PI / 180;
  const cp = Math.cos(pitchRad);
  camera.setPosition(
    Math.sin(yawRad) * cp * distance,
    Math.sin(pitchRad) * distance + 0.7,
    Math.cos(yawRad) * cp * distance
  );
  camera.lookAt(0, 0.25, 0);
}

dragonAsset.on("load", (asset) => {
  dragon = asset.resource.instantiateRenderEntity();
  dragon.name = "ResonanceDragon";
  dragonPivot.addChild(dragon);
  dragon.setLocalPosition(0, baseY, 0);
  dragon.setLocalEulerAngles(0, 180, 0);
  dragon.render.castShadows = true;
  frameDragon();
  updateCamera();
  loading.classList.add("hidden");
  status.textContent = "Awake";
  say("Resonance is awake");
});

dragonAsset.on("error", (err) => {
  loading.querySelector("p").textContent = "Dragon asset failed to load";
  status.textContent = "Asset error";
  console.error(err);
});

app.assets.add(dragonAsset);
app.assets.load(dragonAsset);

app.on("update", (dt) => {
  if (!dragon || paused) return;
  const t = performance.now() * 0.001;
  dragon.setLocalPosition(0, baseY + Math.sin(t * 1.35) * 0.035, 0);
  dragonPivot.rotateLocal(0, Math.sin(t * 0.18) * 0.012, 0);
});

function pointerStart(x, y) {
  dragging = true;
  lastX = x;
  lastY = y;
}
function pointerMove(x, y) {
  if (!dragging) return;
  yaw += (x - lastX) * 0.28;
  pitch = Math.max(-30, Math.min(20, pitch - (y - lastY) * 0.18));
  lastX = x;
  lastY = y;
  updateCamera();
}
function pointerEnd() { dragging = false; }

canvas.addEventListener("pointerdown", (e) => pointerStart(e.clientX, e.clientY));
canvas.addEventListener("pointermove", (e) => pointerMove(e.clientX, e.clientY));
window.addEventListener("pointerup", pointerEnd);

let lastPinch = null;
canvas.addEventListener("touchmove", (e) => {
  if (e.touches.length !== 2) return;
  e.preventDefault();
  const a = e.touches[0], b = e.touches[1];
  const d = Math.hypot(a.clientX - b.clientX, a.clientY - b.clientY);
  if (lastPinch !== null) {
    distance = Math.max(4, Math.min(14, distance - (d - lastPinch) * 0.012));
    updateCamera();
  }
  lastPinch = d;
}, { passive: false });
canvas.addEventListener("touchend", () => { lastPinch = null; });

resetButton.addEventListener("click", () => {
  yaw = 18; pitch = -7; distance = 7.2;
  frameDragon(); updateCamera(); say("View reset");
});
pauseButton.addEventListener("click", () => {
  paused = !paused;
  pauseButton.textContent = paused ? "Resume idle" : "Pause idle";
  say(paused ? "Idle paused" : "Idle resumed");
});

canvas.addEventListener("dblclick", () => {
  if (!dragon) return;
  paused = !paused;
  pauseButton.textContent = paused ? "Resume idle" : "Pause idle";
});

app.start();
