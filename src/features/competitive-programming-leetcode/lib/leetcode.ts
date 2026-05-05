import { promises as fs } from 'node:fs';
import path from 'node:path';

const LEETCODE_ROOT = path.resolve(process.cwd(), 'competitive_programming', 'leetcode');
const LEETCODE_ASSET_PREFIX = '/competitive-programming-assets/leetcode/';
const LEETCODE_ROUTE_PREFIX = '/competitive-programming/leetcode';

type LeetcodeProblemStatus = 'solved' | 'planned';
type LeetcodeTone = 'warm' | 'cool' | 'olive' | 'expert';

export type LeetcodeDifficulty = 'Easy' | 'Medium' | 'Hard';

export interface LeetcodeLearningPathStep {
	title: string;
	description: string;
	categories: string[];
}

export interface LeetcodeCategoryMetadata {
	slug: string;
	title: string;
	description: string;
	focus: string;
	order: number;
	tone?: LeetcodeTone;
}

export interface LeetcodeArchiveMetadata {
	title: string;
	description: string;
	statementPolicy: string;
	positioning: string;
	startHere?: {
		category: string;
		slug: string;
		label?: string;
		description?: string;
	};
	learningPath: LeetcodeLearningPathStep[];
	categories: LeetcodeCategoryMetadata[];
}

interface LeetcodeProblemMeta {
	title: string;
	category: string;
	slug: string;
	officialUrl: string;
	difficulty: LeetcodeDifficulty;
	tags: string[];
	status: LeetcodeProblemStatus;
	order: number;
	summary: string;
}

export interface LeetcodeProblemRecord extends LeetcodeProblemMeta {
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

export interface LeetcodeProblemSourceBundle {
	editorial: string;
	cpp: string;
}

export interface LeetcodeCategoryRecord extends LeetcodeCategoryMetadata {
	records: LeetcodeProblemRecord[];
	problemCount: number;
	latestUpdated?: Date;
	tags: string[];
	difficultyCounts: Record<LeetcodeDifficulty, number>;
}

export interface LeetcodeArchiveSummary {
	title: string;
	entryCount: number;
	categoryCount: number;
	activeCategoryCount: number;
	latestUpdated?: Date;
	tagCount: number;
	difficultyCounts: Record<LeetcodeDifficulty, number>;
}

let metadataPromise: Promise<LeetcodeArchiveMetadata> | undefined;
let recordsPromise: Promise<LeetcodeProblemRecord[]> | undefined;
let categoryRecordsPromise: Promise<LeetcodeCategoryRecord[]> | undefined;
const sourcePromiseCache = new Map<string, Promise<LeetcodeProblemSourceBundle>>();

function getRelativePath(absolutePath: string) {
	return path.relative(process.cwd(), absolutePath);
}

function getAssetKey(absolutePath: string) {
	return path.relative(LEETCODE_ROOT, absolutePath).replaceAll('\\', '/');
}

function toAssetUrl(assetKey: string) {
	return `${LEETCODE_ASSET_PREFIX}${assetKey}`;
}

function getMaxDate(dates: Date[]) {
	return dates.reduce((latest, current) => (current > latest ? current : latest), dates[0]);
}

function sortCategories(left: LeetcodeCategoryMetadata, right: LeetcodeCategoryMetadata) {
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
	const metadataPath = path.join(LEETCODE_ROOT, 'metadata.json');
	const raw = await fs.readFile(metadataPath, 'utf8');
	const metadata = JSON.parse(raw) as LeetcodeArchiveMetadata;

	return {
		...metadata,
		categories: [...metadata.categories].sort(sortCategories),
	} satisfies LeetcodeArchiveMetadata;
}

async function getCategoryDirectoryEntries(categorySlug: string) {
	const categoryRoot = path.join(LEETCODE_ROOT, categorySlug);
	if (!(await pathExists(categoryRoot))) {
		return [];
	}

	return fs.readdir(categoryRoot, { withFileTypes: true });
}

async function loadProblemRecord(
	category: LeetcodeCategoryMetadata,
	folderName: string,
): Promise<LeetcodeProblemRecord | undefined> {
	const baseDirectory = path.join(LEETCODE_ROOT, category.slug, folderName);
	const metaPath = path.join(baseDirectory, 'meta.json');
	if (!(await pathExists(metaPath))) {
		return undefined;
	}

	const [metaRaw, directoryEntries] = await Promise.all([fs.readFile(metaPath, 'utf8'), fs.readdir(baseDirectory)]);
	const meta = JSON.parse(metaRaw) as LeetcodeProblemMeta;

	if (meta.status !== 'solved') {
		return undefined;
	}

	if (meta.slug !== folderName) {
		throw new Error(
			`LeetCode slug mismatch in ${getRelativePath(metaPath)}: expected "${folderName}", received "${meta.slug}".`,
		);
	}

	if (meta.category !== category.title) {
		throw new Error(
			`LeetCode category mismatch in ${getRelativePath(metaPath)}: expected "${category.title}", received "${meta.category}".`,
		);
	}

	const editorialPath = path.join(baseDirectory, 'editorial.tex');
	const cppPath = path.join(baseDirectory, 'solution.cpp');
	await Promise.all([fs.access(editorialPath), fs.access(cppPath)]);

	const stats = await Promise.all(directoryEntries.map((entry) => fs.stat(path.join(baseDirectory, entry))));
	const updated = getMaxDate(stats.map((stat) => stat.mtime));
	const relativeEditorialPath = getRelativePath(editorialPath);
	const relativeCppPath = getRelativePath(cppPath);
	const relativeMetaPath = getRelativePath(metaPath);

	return {
		...meta,
		id: `leetcode-${category.slug}-${meta.slug}`,
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
			editorial: toAssetUrl(getAssetKey(editorialPath)),
			cpp: toAssetUrl(getAssetKey(cppPath)),
			meta: toAssetUrl(getAssetKey(metaPath)),
		},
	} satisfies LeetcodeProblemRecord;
}

async function loadProblemRecords() {
	const metadata = await getLeetcodeArchiveMetadata();
	const categoryRecords = await Promise.all(
		metadata.categories.map(async (category) => {
			const problemEntries = await getCategoryDirectoryEntries(category.slug);
			const folderNames = problemEntries
				.filter((entry) => entry.isDirectory())
				.map((entry) => entry.name)
				.sort((left, right) => left.localeCompare(right));
			const records = await Promise.all(folderNames.map((folderName) => loadProblemRecord(category, folderName)));

			return records.filter((record): record is LeetcodeProblemRecord => Boolean(record));
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

function createDifficultyCounts(): Record<LeetcodeDifficulty, number> {
	return {
		Easy: 0,
		Medium: 0,
		Hard: 0,
	};
}

async function loadCategoryRecords() {
	const [metadata, records] = await Promise.all([getLeetcodeArchiveMetadata(), getLeetcodeProblemRecords()]);

	return metadata.categories.map((category) => {
		const categoryRecords = records.filter((record) => record.categorySlug === category.slug);
		const difficultyCounts = createDifficultyCounts();
		for (const record of categoryRecords) {
			difficultyCounts[record.difficulty] += 1;
		}

		const latestUpdated = categoryRecords.length
			? [...categoryRecords].sort((left, right) => right.updated.getTime() - left.updated.getTime())[0]?.updated
			: undefined;

		return {
			...category,
			records: categoryRecords,
			problemCount: categoryRecords.length,
			latestUpdated,
			tags: [...new Set(categoryRecords.flatMap((record) => record.tags))].sort((left, right) =>
				left.localeCompare(right),
			),
			difficultyCounts,
		} satisfies LeetcodeCategoryRecord;
	});
}

export async function getLeetcodeArchiveMetadata() {
	if (!metadataPromise) {
		metadataPromise = loadArchiveMetadata();
	}

	return metadataPromise;
}

export async function getLeetcodeProblemRecords() {
	if (!recordsPromise) {
		recordsPromise = loadProblemRecords();
	}

	return recordsPromise;
}

export async function getLeetcodeProblemRecord(categorySlug: string, slug: string) {
	const records = await getLeetcodeProblemRecords();
	return records.find((record) => record.categorySlug === categorySlug && record.slug === slug);
}

export async function getLeetcodeProblemSources(record: LeetcodeProblemRecord) {
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

export async function getLeetcodeCategoryRecords() {
	if (!categoryRecordsPromise) {
		categoryRecordsPromise = loadCategoryRecords();
	}

	return categoryRecordsPromise;
}

export async function getLeetcodeCategoryRecord(categorySlug: string) {
	const categories = await getLeetcodeCategoryRecords();
	return categories.find((category) => category.slug === categorySlug);
}

export async function getLeetcodeActiveCategoryRecords() {
	return (await getLeetcodeCategoryRecords()).filter((category) => category.problemCount > 0);
}

export async function getLeetcodeArchiveSummary(): Promise<LeetcodeArchiveSummary> {
	const [metadata, categories, records] = await Promise.all([
		getLeetcodeArchiveMetadata(),
		getLeetcodeCategoryRecords(),
		getLeetcodeProblemRecords(),
	]);
	const activeCategories = categories.filter((category) => category.problemCount > 0);
	const difficultyCounts = createDifficultyCounts();
	for (const record of records) {
		difficultyCounts[record.difficulty] += 1;
	}

	const latestUpdated = records.length
		? [...records].sort((left, right) => right.updated.getTime() - left.updated.getTime())[0]?.updated
		: undefined;

	return {
		title: metadata.title,
		entryCount: records.length,
		categoryCount: categories.length,
		activeCategoryCount: activeCategories.length,
		latestUpdated,
		tagCount: new Set(records.flatMap((record) => record.tags)).size,
		difficultyCounts,
	};
}

export function getLeetcodeRoutePath(categorySlug?: string, slug?: string) {
	if (categorySlug && slug) {
		return `${LEETCODE_ROUTE_PREFIX}/${categorySlug}/${slug}`;
	}
	if (categorySlug) {
		return `${LEETCODE_ROUTE_PREFIX}/${categorySlug}`;
	}
	return LEETCODE_ROUTE_PREFIX;
}

export function getLeetcodeDifficultyTone(difficulty: LeetcodeDifficulty): LeetcodeTone {
	switch (difficulty) {
		case 'Easy':
			return 'olive';
		case 'Medium':
			return 'cool';
		case 'Hard':
			return 'expert';
	}
}

export async function getLeetcodeAssetMap() {
	const records = await getLeetcodeProblemRecords();
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

	addAsset(path.join(LEETCODE_ROOT, 'metadata.json'));
	addAsset(path.join(LEETCODE_ROOT, 'STRUCTURE.md'));

	for (const record of records) {
		addAsset(path.resolve(process.cwd(), record.sourcePaths.editorial));
		addAsset(path.resolve(process.cwd(), record.sourcePaths.cpp));
		addAsset(path.resolve(process.cwd(), record.sourcePaths.meta));
	}

	return assets;
}
