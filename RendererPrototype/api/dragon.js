const DRAGON_URL = 'https://github.com/christinejury2006-ctrl/Resonance./releases/download/renderer-assets/Resonance.glb';

export default async function handler(request, response) {
  try {
    const upstream = await fetch(DRAGON_URL, {
      redirect: 'follow',
      headers: { 'Accept': 'application/octet-stream' }
    });
    if (!upstream.ok) {
      response.status(upstream.status).send('Resonance asset unavailable');
      return;
    }
    response.statusCode = 200;
    response.setHeader('Content-Type', 'model/gltf-binary');
    response.setHeader('Cache-Control', 'public, max-age=3600, s-maxage=86400');
    response.setHeader('Access-Control-Allow-Origin', '*');
    response.send(Buffer.from(await upstream.arrayBuffer()));
  } catch (error) {
    console.error('Resonance asset proxy failed:', error);
    response.status(502).send('Resonance asset proxy failed');
  }
}