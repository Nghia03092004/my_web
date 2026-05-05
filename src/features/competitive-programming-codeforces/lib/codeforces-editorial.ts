export interface CodeforcesEditorialSection {
	title: string;
	content: string;
}

export interface CodeforcesEditorialDocument {
	summary?: CodeforcesEditorialSection;
	editorial?: CodeforcesEditorialSection;
	pseudocode?: CodeforcesEditorialSection;
	complexity?: CodeforcesEditorialSection;
	notes?: CodeforcesEditorialSection;
	extras: CodeforcesEditorialSection[];
}

const SECTION_PATTERN = /\\section\*?\{([^}]*)\}/g;
const SUMMARY_PATTERN = /^(problem summary|summary)$/i;
const EDITORIAL_PATTERN = /^(editorial|solution|idea|approach)$/i;
const PSEUDOCODE_PATTERN = /^pseudocode$/i;
const COMPLEXITY_PATTERN = /^complexity(?: analysis)?$/i;
const NOTES_PATTERN = /^(notes|pitfalls|notes and pitfalls|pitfalls and notes|notes \/ pitfalls)$/i;

function normalizeTitle(value: string) {
	return value.trim().replace(/\s+/g, ' ');
}

function splitSections(source: string) {
	const normalized = source.replace(/\r\n/g, '\n').trim();
	const matches = [...normalized.matchAll(SECTION_PATTERN)];
	if (!matches.length) {
		return [] as CodeforcesEditorialSection[];
	}

	return matches.map((match, index) => {
		const start = (match.index ?? 0) + match[0].length;
		const end = matches[index + 1]?.index ?? normalized.length;

		return {
			title: normalizeTitle(match[1] ?? ''),
			content: normalized.slice(start, end).trim(),
		} satisfies CodeforcesEditorialSection;
	});
}

export function parseCodeforcesEditorial(texSource: string): CodeforcesEditorialDocument {
	const sections = splitSections(texSource);
	if (!sections.length) {
		return {
			editorial: {
				title: 'Editorial',
				content: texSource.trim(),
			},
			extras: [],
		};
	}

	let extras: CodeforcesEditorialSection[] = [];
	let summary: CodeforcesEditorialSection | undefined;
	let editorial: CodeforcesEditorialSection | undefined;
	let pseudocode: CodeforcesEditorialSection | undefined;
	let complexity: CodeforcesEditorialSection | undefined;
	let notes: CodeforcesEditorialSection | undefined;

	for (const section of sections) {
		if (!summary && SUMMARY_PATTERN.test(section.title)) {
			summary = section;
			continue;
		}
		if (!editorial && EDITORIAL_PATTERN.test(section.title)) {
			editorial = section;
			continue;
		}
		if (!pseudocode && PSEUDOCODE_PATTERN.test(section.title)) {
			pseudocode = section;
			continue;
		}
		if (!complexity && COMPLEXITY_PATTERN.test(section.title)) {
			complexity = section;
			continue;
		}
		if (!notes && NOTES_PATTERN.test(section.title)) {
			notes = section;
			continue;
		}

		extras = [...extras, section];
	}

	if (!editorial && extras.length) {
		[editorial, ...extras] = extras;
	}

	return {
		summary,
		editorial,
		pseudocode,
		complexity,
		notes,
		extras,
	};
}
