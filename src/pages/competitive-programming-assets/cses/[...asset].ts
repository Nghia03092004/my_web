import { promises as fs } from 'node:fs';

import type { APIRoute } from 'astro';

import { getCsesAssetMap } from '../../../features/competitive-programming-cses/lib/cses';

interface AssetProps {
	absolutePath: string;
	contentType: string;
}

export async function getStaticPaths() {
	const assets = await getCsesAssetMap();

	return [...assets.entries()].map(([asset, value]) => ({
		params: {
			asset,
		},
		props: value,
	}));
}

export const GET: APIRoute<AssetProps> = async ({ props }) => {
	const body = await fs.readFile(props.absolutePath);

	return new Response(body, {
		headers: {
			'Content-Type': props.contentType,
		},
	});
};
