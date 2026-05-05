import { promises as fs } from 'node:fs';
import path from 'node:path';

const CODEFORCES_3500_ROOT = path.resolve(
	process.cwd(),
	'competitive_programming',
	'codeforces',
	'3500',
);
const CODEFORCES_3500_PROBLEMS_ROOT = path.join(CODEFORCES_3500_ROOT, 'problems');
const CODEFORCES_3500_ASSET_PREFIX = '/competitive-programming-assets/codeforces/3500/';

type CodeforcesProblemStatus = 'solved' | 'planned';

export interface Codeforces3500ArchiveMetadata {
	title: string;
	description: string;
	statementPolicy: string;
	startHere?: {
		slug: string;
		label?: string;
		description?: string;
	};
}

interface Codeforces3500ProblemMeta {
	title: string;
	slug: string;
	contestId: number;
	index: string;
	rating: number;
	officialUrl: string;
	tags: string[];
	status: CodeforcesProblemStatus;
	order: number;
	summary: string;
}

export interface Codeforces3500ProblemRecord extends Codeforces3500ProblemMeta {
	id: string;
	contestLabel: string;
	updated: Date;
	sourceDirectory: string;
	sourcePaths: {
		editorial: string;
		cpp: string;
		meta: string;
	};
	assetUrls: {
		editorial: string;
		cpp: string;
		meta: string;
	};
}

export interface Codeforces3500ProblemSourceBundle {
	editorial: string;
	cpp: string;
}

export interface Codeforces3500ArchiveSummary {
	title: string;
	entryCount: number;
	latestUpdated?: Date;
	tagCount: number;
}

let metadataPromise: Promise<Codeforces3500ArchiveMetadata> | undefined;
let recordsPromise: Promise<Codeforces3500ProblemRecord[]> | undefined;
const sourcePromiseCache = new Map<string, Promise<Codeforces3500ProblemSourceBundle>>();

function getRelativePath(absolutePath: string) {
	return path.relative(process.cwd(), absolutePath);
}

function getAssetKey(absolutePath: string) {
	return path.relative(CODEFORCES_3500_ROOT, absolutePath).replaceAll('\\', '/');
}

function toAssetUrl(assetKey: string) {
	return `${CODEFORCES_3500_ASSET_PREFIX}${assetKey}`;
}

function getMaxDate(dates: Date[]) {
	return dates.reduce((latest, current) => (current > latest ? current : latest), dates[0]);
}

async function pathExists(targetPath: string) {
	return fs
		.access(targetPath)
		.then(() => true)
		.catch(() => false);
}

async function loadArchiveMetadata() {
	const metadataPath = path.join(CODEFORCES_3500_ROOT, 'metadata.json');
	const raw = await fs.readFile(metadataPath, 'utf8');
	return JSON.parse(raw) as Codeforces3500ArchiveMetadata;
}

async function loadProblemRecord(folderName: string): Promise<Codeforces3500ProblemRecord | undefined> {
	const baseDirectory = path.join(CODEFORCES_3500_PROBLEMS_ROOT, folderName);
	const metaPath = path.join(baseDirectory, 'meta.json');
	if (!(await pathExists(metaPath))) {
		return undefined;
	}

	const [metaRaw, directoryEntries] = await Promise.all([
		fs.readFile(metaPath, 'utf8'),
		fs.readdir(baseDirectory),
	]);
	const meta = JSON.parse(metaRaw) as Codeforces3500ProblemMeta;

	if (meta.status !== 'solved') {
		return undefined;
	}

	if (meta.slug !== folderName) {
		throw new Error(
			`Codeforces 3500+ slug mismatch in ${getRelativePath(metaPath)}: expected "${folderName}", received "${meta.slug}".`,
		);
	}

	const editorialPath = path.join(baseDirectory, 'editorial.tex');
	const cppPath = path.join(baseDirectory, 'solution.cpp');
	await Promise.all([fs.access(editorialPath), fs.access(cppPath)]);

	const stats = await Promise.all(
		directoryEntries.map((entry) => fs.stat(path.join(baseDirectory, entry))),
	);
	const updated = getMaxDate(stats.map((stat) => stat.mtime));
	const contestLabel = `${meta.contestId}${meta.index}`;
	const relativeEditorialPath = getRelativePath(editorialPath);
	const relativeCppPath = getRelativePath(cppPath);
	const relativeMetaPath = getRelativePath(metaPath);
	const editorialAssetKey = getAssetKey(editorialPath);
	const cppAssetKey = getAssetKey(cppPath);
	const metaAssetKey = getAssetKey(metaPath);

	return {
		...meta,
		id: `codeforces-3500-${meta.contestId}-${meta.index.toLowerCase()}-${meta.slug}`,
		contestLabel,
		updated,
		sourceDirectory: getRelativePath(baseDirectory),
		sourcePaths: {
			editorial: relativeEditorialPath,
			cpp: relativeCppPath,
			meta: relativeMetaPath,
		},
		assetUrls: {
			editorial: toAssetUrl(editorialAssetKey),
			cpp: toAssetUrl(cppAssetKey),
			meta: toAssetUrl(metaAssetKey),
		},
	} satisfies Codeforces3500ProblemRecord;
}

async function loadProblemRecords() {
	const entries = await fs.readdir(CODEFORCES_3500_PROBLEMS_ROOT, { withFileTypes: true });
	const folderNames = entries
		.filter((entry) => entry.isDirectory())
		.map((entry) => entry.name)
		.sort((left, right) => left.localeCompare(right));
	const records = await Promise.all(folderNames.map((folderName) => loadProblemRecord(folderName)));

	return records
		.filter((record): record is Codeforces3500ProblemRecord => Boolean(record))
		.sort((left, right) => {
			if (left.order !== right.order) {
				return left.order - right.order;
			}
			if (left.rating !== right.rating) {
				return right.rating - left.rating;
			}
			if (left.contestId !== right.contestId) {
				return left.contestId - right.contestId;
			}
			return left.title.localeCompare(right.title);
		});
}

export async function getCodeforces3500ArchiveMetadata() {
	if (!metadataPromise) {
		metadataPromise = loadArchiveMetadata();
	}

	return metadataPromise;
}

export async function getCodeforces3500ProblemRecords() {
	if (!recordsPromise) {
		recordsPromise = loadProblemRecords();
	}

	return recordsPromise;
}

export async function getCodeforces3500ProblemRecord(slug: string) {
	const records = await getCodeforces3500ProblemRecords();
	return records.find((record) => record.slug === slug);
}

export async function getCodeforces3500ProblemSources(record: Codeforces3500ProblemRecord) {
	const cached = sourcePromiseCache.get(record.id);
	if (cached) {
		return cached;
	}

	const promise = Promise.all([
		fs.readFile(path.resolve(process.cwd(), record.sourcePaths.editorial), 'utf8'),
		fs.readFile(path.resolve(process.cwd(), record.sourcePaths.cpp), 'utf8'),
	]).then(([editorial, cpp]) => ({
		editorial,
		cpp,
	}));

	sourcePromiseCache.set(record.id, promise);
	return promise;
}

export async function getCodeforces3500ArchiveSummary(): Promise<Codeforces3500ArchiveSummary> {
	const [metadata, records] = await Promise.all([
		getCodeforces3500ArchiveMetadata(),
		getCodeforces3500ProblemRecords(),
	]);
	const latestUpdated = records.length
		? [...records].sort((left, right) => right.updated.getTime() - left.updated.getTime())[0]?.updated
		: undefined;
	const tagCount = new Set(records.flatMap((record) => record.tags)).size;

	return {
		title: metadata.title,
		entryCount: records.length,
		latestUpdated,
		tagCount,
	};
}

export async function getCodeforces3500AssetMap() {
	const records = await getCodeforces3500ProblemRecords();
	const assets = new Map<
		string,
		{
			absolutePath: string;
			contentType: string;
		}
	>();

	const getContentType = (assetKey: string) => {
		if (assetKey.endsWith('.json')) return 'application/json; charset=utf-8';
		if (assetKey.endsWith('.md') || assetKey.endsWith('.tex') || assetKey.endsWith('.cpp')) {
			return 'text/plain; charset=utf-8';
		}

		return 'application/octet-stream';
	};

	const addAsset = (absolutePath: string) => {
		const assetKey = getAssetKey(absolutePath);
		if (assets.has(assetKey)) {
			return;
		}

		assets.set(assetKey, {
			absolutePath,
			contentType: getContentType(assetKey),
		});
	};

	addAsset(path.join(CODEFORCES_3500_ROOT, 'metadata.json'));
	addAsset(path.join(CODEFORCES_3500_ROOT, 'STRUCTURE.md'));

	for (const record of records) {
		addAsset(path.resolve(process.cwd(), record.sourcePaths.editorial));
		addAsset(path.resolve(process.cwd(), record.sourcePaths.cpp));
		addAsset(path.resolve(process.cwd(), record.sourcePaths.meta));
	}

	return assets;
}
