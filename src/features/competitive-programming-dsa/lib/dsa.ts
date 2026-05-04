import { promises as fs } from 'node:fs';
import path from 'node:path';

const DSA_ROOT = path.resolve(process.cwd(), 'competitive_programming', 'dsa');
const DSA_ASSET_PREFIX = '/competitive-programming-assets/dsa/';
const DSA_ROUTE_PREFIX = '/competitive-programming/data-structures-and-algorithms';

export type DsaLevel = 'basic' | 'intermediate' | 'advanced' | 'expert';

export interface DsaTopicDefinition {
	title: string;
	status: 'planned' | 'outline';
}

export interface DsaCategoryDefinition {
	slug: DsaCategorySlug;
	title: string;
	description: string;
	longDescription: string;
	plannedTopics: DsaTopicDefinition[];
}

export interface DsaNoteMeta {
	title: string;
	summary: string;
	level: DsaLevel;
	tags: string[];
	order?: number;
}

export interface DsaNoteRecord {
	id: string;
	slug: string;
	title: string;
	summary: string;
	level: DsaLevel;
	tags: string[];
	order: number;
	categorySlug: DsaCategorySlug;
	categoryTitle: string;
	sourceDirectory: string;
	sourcePaths: {
		tex: string;
		cpp: string;
		figures: string[];
	};
	assetUrls: {
		tex: string;
		cpp: string;
		figures: string[];
		assetBase: string;
	};
	updated: Date;
}

export interface DsaCategoryRecord extends DsaCategoryDefinition {
	introSourcePath: string;
	introAssetUrl: string;
	notes: DsaNoteRecord[];
	noteCount: number;
}

export interface DsaLandingSources {
	introTex: string;
	taxonomyTex: string;
	sourcePaths: {
		intro: string;
		taxonomy: string;
	};
	assetUrls: {
		intro: string;
		taxonomy: string;
	};
}

const CATEGORY_DEFINITIONS = [
	{
		slug: 'fundamentals',
		title: 'Fundamentals',
		description: 'Core patterns that show up before the heavy machinery does.',
		longDescription:
			'These are the habits and small techniques that decide whether a problem is a quick implementation or an unnecessary detour.',
		plannedTopics: [
			{ title: 'Complexity analysis', status: 'planned' },
			{ title: 'Sorting', status: 'planned' },
			{ title: 'Sweep line basics', status: 'outline' },
		],
	},
	{
		slug: 'data-structures',
		title: 'Data Structures',
		description: 'Range-query workhorses, balanced trees, and dynamic forest tools.',
		longDescription:
			'This branch focuses on structures that trade memory, invariants, or amortization for faster updates and queries.',
		plannedTopics: [
			{ title: 'Priority queue', status: 'planned' },
			{ title: 'Persistent DSU', status: 'outline' },
			{ title: 'Segment tree beats', status: 'outline' },
		],
	},
	{
		slug: 'graph-algorithms',
		title: 'Graph Algorithms',
		description: 'Traversal, shortest paths, tree decompositions, and structure-finding routines.',
		longDescription:
			'When a problem hides an interaction graph, these notes aim to make the graph structure explicit instead of magical.',
		plannedTopics: [
			{ title: 'Bellman-Ford', status: 'planned' },
			{ title: 'Floyd-Warshall', status: 'planned' },
			{ title: 'DSU on tree', status: 'planned' },
			{ title: 'Dominator tree', status: 'outline' },
		],
	},
	{
		slug: 'dynamic-programming',
		title: 'Dynamic Programming',
		description: 'State design, transitions, and the optimizations that make large DP feasible.',
		longDescription:
			'The focus here is less on memorizing formulas and more on seeing what information must survive from one decision to the next.',
		plannedTopics: [
			{ title: 'Classical DP', status: 'planned' },
			{ title: 'Interval DP', status: 'planned' },
			{ title: 'Subset convolution', status: 'outline' },
			{ title: 'Slope trick', status: 'outline' },
		],
	},
	{
		slug: 'number-theory',
		title: 'Number Theory',
		description: 'Modular arithmetic, multiplicative structure, and fast polynomial machinery.',
		longDescription:
			'These notes emphasize the parts of number theory that become algorithms rather than standalone proofs.',
		plannedTopics: [
			{ title: 'Inclusion-exclusion', status: 'planned' },
			{ title: 'Garner algorithm', status: 'outline' },
			{ title: 'Multiplicative functions', status: 'outline' },
			{ title: 'Berlekamp-Massey', status: 'outline' },
		],
	},
	{
		slug: 'string-algorithms',
		title: 'String Algorithms',
		description: 'Prefix structure, automata, and linear-time matching tools.',
		longDescription:
			'String problems are often bookkeeping problems in disguise. The goal is to keep the invariant small enough to update in linear time.',
		plannedTopics: [
			{ title: 'Palindromic tree', status: 'planned' },
			{ title: 'Suffix tree', status: 'outline' },
		],
	},
	{
		slug: 'geometry',
		title: 'Geometry',
		description: 'Reliable primitives first, then hulls, sweeps, and polygon routines.',
		longDescription:
			'The intended emphasis is contest geometry that survives integer arithmetic, not coordinate-free elegance for its own sake.',
		plannedTopics: [
			{ title: 'Half-plane intersection', status: 'planned' },
			{ title: 'Sweep line', status: 'outline' },
			{ title: 'KD-tree', status: 'outline' },
		],
	},
	{
		slug: 'flows-and-matching',
		title: 'Flows and Matching',
		description: 'Network formulations for path packing, assignment, and constrained transfer.',
		longDescription:
			'This branch collects problems where the cleanest solution is to phrase the whole task as conserved flow or structural matching.',
		plannedTopics: [
			{ title: 'Hungarian algorithm', status: 'outline' },
			{ title: 'General matching', status: 'outline' },
		],
	},
	{
		slug: 'advanced-tricks',
		title: 'Advanced Tricks',
		description: 'Offline reordering, amortized arguments, and techniques that show up when standard tools are almost enough.',
		longDescription:
			'These topics are the part of competitive programming that is hardest to compress into one sentence but easiest to recognize after enough practice.',
		plannedTopics: [
			{ title: 'Divide and conquer on answer', status: 'planned' },
			{ title: 'Randomization', status: 'planned' },
			{ title: 'Amortized analysis', status: 'planned' },
			{ title: 'Potential method', status: 'planned' },
			{ title: 'Parametric search', status: 'planned' },
		],
	},
] as const satisfies readonly DsaCategoryDefinition[];

export type DsaCategorySlug = (typeof CATEGORY_DEFINITIONS)[number]['slug'];

let categoryRecordsPromise: Promise<DsaCategoryRecord[]> | undefined;
let landingSourcesPromise: Promise<DsaLandingSources> | undefined;
let assetMapPromise:
	| Promise<
			Map<
				string,
				{
					absolutePath: string;
					contentType: string;
				}
			>
	  >
	| undefined;

function getRelativePath(absolutePath: string) {
	return path.relative(process.cwd(), absolutePath);
}

function toAssetUrl(relativePath: string) {
	const normalized = relativePath.replace(/^competitive_programming[\\/]/, '').replaceAll('\\', '/');
	return `${DSA_ASSET_PREFIX}${normalized.replace(/^dsa\//, '')}`;
}

function toRoutePath(categorySlug?: string, noteSlug?: string) {
	if (categorySlug && noteSlug) {
		return `${DSA_ROUTE_PREFIX}/${categorySlug}/${noteSlug}`;
	}
	if (categorySlug) {
		return `${DSA_ROUTE_PREFIX}/${categorySlug}`;
	}
	return DSA_ROUTE_PREFIX;
}

async function pathExists(targetPath: string) {
	return fs
		.access(targetPath)
		.then(() => true)
		.catch(() => false);
}

async function collectFiles(directory: string): Promise<string[]> {
	const entries = await fs.readdir(directory, { withFileTypes: true });
	const nested = await Promise.all(
		entries.map(async (entry) => {
			const absolutePath = path.join(directory, entry.name);
			if (entry.isDirectory()) {
				return collectFiles(absolutePath);
			}
			return [absolutePath];
		}),
	);
	return nested.flat();
}

async function getDirectoryUpdatedAt(directory: string) {
	const files = await collectFiles(directory);
	const stats = await Promise.all(files.map((filePath) => fs.stat(filePath)));
	return stats.reduce((latest, current) => (current.mtime > latest ? current.mtime : latest), stats[0]?.mtime ?? new Date());
}

async function readJsonFile<T>(absolutePath: string) {
	return JSON.parse(await fs.readFile(absolutePath, 'utf8')) as T;
}

function compareNotes(left: DsaNoteRecord, right: DsaNoteRecord) {
	if (left.order !== right.order) {
		return left.order - right.order;
	}
	return left.title.localeCompare(right.title);
}

async function loadCategoryRecord(definition: DsaCategoryDefinition): Promise<DsaCategoryRecord> {
	const categoryRoot = path.join(DSA_ROOT, definition.slug);
	const introAbsolutePath = path.join(categoryRoot, 'index.tex');
	const entries = await fs.readdir(categoryRoot, { withFileTypes: true });
	const noteDirectories = entries
		.filter((entry) => entry.isDirectory())
		.map((entry) => entry.name)
		.sort((left, right) => left.localeCompare(right));

	const notes = await Promise.all(
		noteDirectories.map(async (noteSlug) => {
			const noteRoot = path.join(categoryRoot, noteSlug);
			const metaAbsolutePath = path.join(noteRoot, 'meta.json');
			const texAbsolutePath = path.join(noteRoot, 'note.tex');
			const cppAbsolutePath = path.join(noteRoot, 'code.cpp');

			if (!(await pathExists(metaAbsolutePath)) || !(await pathExists(texAbsolutePath)) || !(await pathExists(cppAbsolutePath))) {
				throw new Error(`Incomplete DSA note folder: ${noteRoot}`);
			}

			const meta = await readJsonFile<DsaNoteMeta>(metaAbsolutePath);
			const figuresRoot = path.join(noteRoot, 'figures');
			const figureRelativePaths = (await pathExists(figuresRoot))
				? (await collectFiles(figuresRoot)).map((filePath) => getRelativePath(filePath))
				: [];
			const sourceDirectory = getRelativePath(noteRoot);

			return {
				id: `${definition.slug}-${noteSlug}`,
				slug: noteSlug,
				title: meta.title,
				summary: meta.summary,
				level: meta.level,
				tags: meta.tags,
				order: meta.order ?? 1000,
				categorySlug: definition.slug,
				categoryTitle: definition.title,
				sourceDirectory,
				sourcePaths: {
					tex: getRelativePath(texAbsolutePath),
					cpp: getRelativePath(cppAbsolutePath),
					figures: figureRelativePaths,
				},
				assetUrls: {
					tex: toAssetUrl(getRelativePath(texAbsolutePath)),
					cpp: toAssetUrl(getRelativePath(cppAbsolutePath)),
					figures: figureRelativePaths.map((relativePath) => toAssetUrl(relativePath)),
					assetBase: toAssetUrl(sourceDirectory),
				},
				updated: await getDirectoryUpdatedAt(noteRoot),
			} satisfies DsaNoteRecord;
		}),
	);

	return {
		...definition,
		introSourcePath: getRelativePath(introAbsolutePath),
		introAssetUrl: toAssetUrl(getRelativePath(introAbsolutePath)),
		notes: notes.sort(compareNotes),
		noteCount: notes.length,
	};
}

async function loadCategoryRecords() {
	return Promise.all(CATEGORY_DEFINITIONS.map((definition) => loadCategoryRecord(definition)));
}

export async function getDsaCategoryRecords() {
	if (!categoryRecordsPromise) {
		categoryRecordsPromise = loadCategoryRecords();
	}

	return categoryRecordsPromise;
}

export async function getDsaCategoryRecord(categorySlug: string) {
	return (await getDsaCategoryRecords()).find((category) => category.slug === categorySlug);
}

export async function getDsaNoteRecords() {
	return (await getDsaCategoryRecords()).flatMap((category) => category.notes);
}

export async function getDsaNoteRecord(categorySlug: string, noteSlug: string) {
	const category = await getDsaCategoryRecord(categorySlug);
	return category?.notes.find((note) => note.slug === noteSlug);
}

export async function getDsaLandingSources() {
	if (!landingSourcesPromise) {
		landingSourcesPromise = Promise.all([
			fs.readFile(path.join(DSA_ROOT, 'index.tex'), 'utf8'),
			fs.readFile(path.join(DSA_ROOT, 'taxonomy.tex'), 'utf8'),
		]).then(([introTex, taxonomyTex]) => ({
			introTex,
			taxonomyTex,
			sourcePaths: {
				intro: getRelativePath(path.join(DSA_ROOT, 'index.tex')),
				taxonomy: getRelativePath(path.join(DSA_ROOT, 'taxonomy.tex')),
			},
			assetUrls: {
				intro: toAssetUrl(getRelativePath(path.join(DSA_ROOT, 'index.tex'))),
				taxonomy: toAssetUrl(getRelativePath(path.join(DSA_ROOT, 'taxonomy.tex'))),
			},
		}));
	}

	return landingSourcesPromise;
}

export async function getDsaCategoryIntroTex(categorySlug: DsaCategorySlug) {
	return fs.readFile(path.join(DSA_ROOT, categorySlug, 'index.tex'), 'utf8');
}

export function getDsaLevelTone(level: DsaLevel) {
	switch (level) {
		case 'basic':
			return 'olive';
		case 'intermediate':
			return 'cool';
		case 'advanced':
			return 'warm';
		case 'expert':
			return 'expert';
	}
}

export function getDsaRoutePath(categorySlug?: string, noteSlug?: string) {
	return toRoutePath(categorySlug, noteSlug);
}

export async function getDsaAssetMap() {
	if (assetMapPromise) {
		return assetMapPromise;
	}

	assetMapPromise = collectFiles(DSA_ROOT).then((files) => {
		const assets = new Map<
			string,
			{
				absolutePath: string;
				contentType: string;
			}
		>();

		const getContentType = (relativePath: string) => {
			if (relativePath.endsWith('.svg')) return 'image/svg+xml';
			if (relativePath.endsWith('.png')) return 'image/png';
			if (relativePath.endsWith('.jpg') || relativePath.endsWith('.jpeg')) return 'image/jpeg';
			if (relativePath.endsWith('.webp')) return 'image/webp';
			if (relativePath.endsWith('.json')) return 'application/json; charset=utf-8';
			if (relativePath.endsWith('.md') || relativePath.endsWith('.txt') || relativePath.endsWith('.tex') || relativePath.endsWith('.cpp')) {
				return 'text/plain; charset=utf-8';
			}
			return 'application/octet-stream';
		};

		for (const absolutePath of files) {
			const relativePath = getRelativePath(absolutePath);
			const assetKey = relativePath.replace(/^competitive_programming[\\/]/, '').replaceAll('\\', '/').replace(/^dsa\//, '');
			assets.set(assetKey, {
				absolutePath,
				contentType: getContentType(relativePath),
			});
		}

		return assets;
	});

	return assetMapPromise;
}
