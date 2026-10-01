# Dragonbound Cinematic Renderer Prototype

Experimental real-time rendering path for Dragonbound. It does not replace the Unreal implementation.

Goal: prove that the supplied war_dragon_rigged.glb can reach a cinematic fantasy look with PlayCanvas Engine 2, WebGPU/WebGL2 fallback, PBR materials, ACES tone mapping, HDR-oriented exposure, clustered lighting, colored key/fill/rim lighting, shadows and a contact-shadow ground.

The prototype loads the existing dragon binary through the repository Git LFS media endpoint instead of duplicating the asset.

Run from RendererPrototype with npm install, then npm run dev.

Next passes: HDR environment lighting, material tuning, atmospheric depth, post-processing, camera/lens tuning, Android WebGPU profiling, then the rider hero shot.

The Unreal branch remains intact while this renderer is evaluated.
