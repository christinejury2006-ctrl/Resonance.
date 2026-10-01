const DRAGON_URL = 'https://github.com/christinejury2006-ctrl/Resonance./releases/download/renderer-assets/war_dragon_rigged.glb';

export default async function handler(request, response) {
  try {
    const upstream = await fetch(DRAGON_URL, {
      redirect: 'follow',
      headers: { 'Accept': 'application/octet-stream' }
    });

    if (!upstream.ok) {
      response.status(upstream.status).send('Dragon asset unavailable');
      return;
    }

    response.statusCode = 200;
    response.setHeader('Content-Type', 'model/gltf-binary');
    response.setHeader('Cache-Control', 'public, max-age=3600, s-maxage=86400');
    response.setHeader('Access-Control-Allow-Origin', '*');

    const buffer = Buffer.from(await upstream.arrayBuffer());
    response.send(buffer);
  } catch (error) {
    console.error('Dragon proxy failed:', error);
    response.status(502).send('Dragon asset proxy failed');
  }
}
