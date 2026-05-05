import { promises as fs } from 'node:fs';
import path from 'node:path';

const CSES_ROOT = path.resolve(process.cwd(), 'competitive_programming', 'cses');
const CSES_ASSET_PREFIX = '/competitive-programming-assets/cses/';

type CsesProblemStatus = 'solved' | 'planned';
type CsesTone = 'warm' | 'cool' | 'olive' | 'expert';

export interface CsesCategoryMetadata {
	slug: string;
	title: string;
	description: string;
	order: number;
	tone?: CsesTone;
}

export interface CsesArchiveMetadata {
	title: string;
	description: string;
	statementPolicy: string;
	startHere?: {
		category: string;
		slug: string;
		label?: string;
		description?: string;
	};
	categories: CsesCategoryMetadata[];
}

interface CsesProblemMeta {
	title: string;
	category: string;
	slug: string;
	officialUrl: string;
	level: string;
	tags: string[];
	status: CsesProblemStatus;
	order: number;
	summary: string;
}

export interface CsesProblemRecord extends CsesProblemMeta {
	id: string;
	categorySlug: string;
	categoryTitle: string;
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

export interface CsesProblemSourceBundle {
	editorial: string;
	cpp: string;
}

export interface CsesCategoryRecord extends CsesCategoryMetadata {
	records: CsesProblemRecord[];
	solvedCount: number;
	latestUpdated?: Date;
	tags: string[];
	levels: string[];
}

export interface CsesArchiveSummary {
	title: string;
	entryCount: number;
	categoryCount: number;
	activeCategoryCount: number;
	latestUpdated?: Date;
}

let metadataPromise: Promise<CsesArchiveMetadata> | undefined;
let recordsPromise: Promise<CsesProblemRecord[]> | undefined;
let categoryRecordsPromise: Promise<CsesCategoryRecord[]> | undefined;
const sourcePromiseCache = new Map<string, Promise<CsesProblemSourceBundle>>();

function getRelativePath(absolutePath: string) {
	return path.relative(process.cwd(), absolutePath);
}

function getAssetKey(absolutePath: string) {
	return path.relative(CSES_ROOT, absolutePath).replaceAll('\\', '/');
}

function toAssetUrl(assetKey: string) {
	return `${CSES_ASSET_PREFIX}${assetKey}`;
}

function getMaxDate(dates: Date[]) {
	return dates.reduce((latest, current) => (current > latest ? current : latest), dates[0]);
}

function sortCategories(left: CsesCategoryMetadata, right: CsesCategoryMetadata) {
	if (left.order !== right.order) {
		return left.order - right.order;
	}

	return left.title.localeCompare(right.title);
}

async function pathExists(targetPath: string) {
	return fs
		.access(targetPath)
		.then(() => true)
		.catch(() => false);
}

async function loadArchiveMetadata() {
	const metadataPath = path.join(CSES_ROOT, 'metadata.json');
	const raw = await fs.readFile(metadataPath, 'utf8');
	const metadata = JSON.parse(raw) as CsesArchiveMetadata;

	return {
		...metadata,
		categories: [...metadata.categories].sort(sortCategories),
	} satisfies CsesArchiveMetadata;
}

async function getCategoryDirectoryEntries(categorySlug: string) {
	const categoryRoot = path.join(CSES_ROOT, categorySlug);
	if (!(await pathExists(categoryRoot))) {
		return [];
	}

	return fs.readdir(categoryRoot, { withFileTypes: true });
}

async function loadProblemRecord(
	category: CsesCategoryMetadata,
	folderName: string,
): Promise<CsesProblemRecord | undefined> {
	const baseDirectory = path.join(CSES_ROOT, category.slug, folderName);
	const metaPath = path.join(baseDirectory, 'meta.json');
	if (!(await pathExists(metaPath))) {
		return undefined;
	}

	const [metaRaw, directoryEntries] = await Promise.all([
		fs.readFile(metaPath, 'utf8'),
		fs.readdir(baseDirectory),
	]);
	const meta = JSON.parse(metaRaw) as CsesProblemMeta;

	if (meta.status !== 'solved') {
		return undefined;
	}

	if (meta.slug !== folderName) {
		throw new Error(
			`CSES slug mismatch in ${getRelativePath(metaPath)}: expected "${folderName}", received "${meta.slug}".`,
		);
	}

	if (meta.category !== category.title) {
		throw new Error(
			`CSES category mismatch in ${getRelativePath(metaPath)}: expected "${category.title}", received "${meta.category}".`,
		);
	}

	const editorialPath = path.join(baseDirectory, 'editorial.tex');
	const cppPath = path.join(baseDirectory, 'solution.cpp');
	await Promise.all([fs.access(editorialPath), fs.access(cppPath)]);

	const stats = await Promise.all(
		directoryEntries.map((entry) => fs.stat(path.join(baseDirectory, entry))),
	);
	const updated = getMaxDate(stats.map((stat) => stat.mtime));
	const relativeEditorialPath = getRelativePath(editorialPath);
	const relativeCppPath = getRelativePath(cppPath);
	const relativeMetaPath = getRelativePath(metaPath);
	const editorialAssetKey = getAssetKey(editorialPath);
	const cppAssetKey = getAssetKey(cppPath);
	const metaAssetKey = getAssetKey(metaPath);

	return {
		...meta,
		id: `cses-${category.slug}-${meta.slug}`,
		categorySlug: category.slug,
		categoryTitle: category.title,
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
	} satisfies CsesProblemRecord;
}

async function loadProblemRecords() {
	const metadata = await getCsesArchiveMetadata();
	const categoryRecords = await Promise.all(
		metadata.categories.map(async (category) => {
			const problemEntries = await getCategoryDirectoryEntries(category.slug);
			const folderNames = problemEntries
				.filter((entry) => entry.isDirectory())
				.map((entry) => entry.name)
				.sort((left, right) => left.localeCompare(right));
			const records = await Promise.all(folderNames.map((folderName) => loadProblemRecord(category, folderName)));

			return records.filter((record): record is CsesProblemRecord => Boolean(record));
		}),
	);
	const categoryOrder = new Map(metadata.categories.map((category) => [category.slug, category.order]));

	return categoryRecords.flat().sort((left, right) => {
		const leftOrder = categoryOrder.get(left.categorySlug) ?? Number.MAX_SAFE_INTEGER;
		const rightOrder = categoryOrder.get(right.categorySlug) ?? Number.MAX_SAFE_INTEGER;
		if (leftOrder !== rightOrder) {
			return leftOrder - rightOrder;
		}
		if (left.order !== right.order) {
			return left.order - right.order;
		}

		return left.title.localeCompare(right.title);
	});
}

async function loadCategoryRecords() {
	const [metadata, records] = await Promise.all([getCsesArchiveMetadata(), getCsesProblemRecords()]);

	return metadata.categories.map((category) => {
		const categoryRecords = records.filter((record) => record.categorySlug === category.slug);
		const latestUpdated = categoryRecords.length
			? [...categoryRecords].sort((left, right) => right.updated.getTime() - left.updated.getTime())[0]?.updated
			: undefined;

		return {
			...category,
			records: categoryRecords,
			solvedCount: categoryRecords.length,
			latestUpdated,
			tags: [...new Set(categoryRecords.flatMap((record) => record.tags))].sort((left, right) =>
				left.localeCompare(right),
			),
			levels: [...new Set(categoryRecords.map((record) => record.level))].sort((left, right) =>
				left.localeCompare(right),
			),
		} satisfies CsesCategoryRecord;
	});
}

export async function getCsesArchiveMetadata() {
	if (!metadataPromise) {
		metadataPromise = loadArchiveMetadata();
	}

	return metadataPromise;
}

export async function getCsesProblemRecords() {
	if (!recordsPromise) {
		recordsPromise = loadProblemRecords();
	}

	return recordsPromise;
}

export async function getCsesProblemRecord(categorySlug: string, slug: string) {
	const records = await getCsesProblemRecords();
	return records.find((record) => record.categorySlug === categorySlug && record.slug === slug);
}

export async function getCsesProblemSources(record: CsesProblemRecord) {
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

export async function getCsesCategoryRecords() {
	if (!categoryRecordsPromise) {
		categoryRecordsPromise = loadCategoryRecords();
	}

	return categoryRecordsPromise;
}

export async function getCsesCategoryRecord(categorySlug: string) {
	const categories = await getCsesCategoryRecords();
	return categories.find((category) => category.slug === categorySlug);
}

export async function getCsesArchiveSummary(): Promise<CsesArchiveSummary> {
	const [metadata, categories] = await Promise.all([getCsesArchiveMetadata(), getCsesCategoryRecords()]);
	const activeCategories = categories.filter((category) => category.solvedCount > 0);
	const latestUpdated = activeCategories.length
		? [...activeCategories]
				.map((category) => category.latestUpdated)
				.filter((value): value is Date => Boolean(value))
				.sort((left, right) => right.getTime() - left.getTime())[0]
		: undefined;

	return {
		title: metadata.title,
		entryCount: categories.reduce((sum, category) => sum + category.solvedCount, 0),
		categoryCount: categories.length,
		activeCategoryCount: activeCategories.length,
		latestUpdated,
	};
}

export async function getCsesAssetMap() {
	const records = await getCsesProblemRecords();
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

	addAsset(path.join(CSES_ROOT, 'metadata.json'));
	addAsset(path.join(CSES_ROOT, 'STRUCTURE.md'));

	for (const record of records) {
		addAsset(path.resolve(process.cwd(), record.sourcePaths.editorial));
		addAsset(path.resolve(process.cwd(), record.sourcePaths.cpp));
		addAsset(path.resolve(process.cwd(), record.sourcePaths.meta));
	}

	return assets;
}
