export interface LeetcodeEditorialSection {
	title: string;
	content: string;
}

export interface LeetcodeEditorialDocument {
	summary?: LeetcodeEditorialSection;
	recognition?: LeetcodeEditorialSection;
	keyIdea?: LeetcodeEditorialSection;
	reasoning?: LeetcodeEditorialSection;
	complexity?: LeetcodeEditorialSection;
	pitfalls?: LeetcodeEditorialSection;
	variants?: LeetcodeEditorialSection;
	extras: LeetcodeEditorialSection[];
}

const SECTION_PATTERN = /\\section\*?\{([^}]*)\}/g;
const SUMMARY_PATTERN = /^(problem summary|summary)$/i;
const RECOGNITION_PATTERN = /^(recognition pattern|pattern recognition|recognition)$/i;
const KEY_IDEA_PATTERN = /^(key idea|idea|approach)$/i;
const REASONING_PATTERN = /^(step[- ]by[- ]step reasoning|reasoning|walkthrough)$/i;
const COMPLEXITY_PATTERN = /^(complexity|complexity analysis)$/i;
const PITFALLS_PATTERN = /^(common pitfalls|pitfalls|notes \/ pitfalls)$/i;
const VARIANTS_PATTERN = /^(variants|follow[- ]ups?|variants \/ follow[- ]ups?)$/i;

function normalizeTitle(value: string) {
	return value.trim().replace(/\s+/g, ' ');
}

function splitSections(source: string) {
	const normalized = source.replace(/\r\n/g, '\n').trim();
	const matches = [...normalized.matchAll(SECTION_PATTERN)];
	if (!matches.length) {
		return [] as LeetcodeEditorialSection[];
	}

	return matches.map((match, index) => {
		const start = (match.index ?? 0) + match[0].length;
		const end = matches[index + 1]?.index ?? normalized.length;

		return {
			title: normalizeTitle(match[1] ?? ''),
			content: normalized.slice(start, end).trim(),
		} satisfies LeetcodeEditorialSection;
	});
}

export function parseLeetcodeEditorial(texSource: string): LeetcodeEditorialDocument {
	const sections = splitSections(texSource);
	if (!sections.length) {
		return {
			summary: {
				title: 'Problem Summary',
				content: texSource.trim(),
			},
			extras: [],
		};
	}

	let extras: LeetcodeEditorialSection[] = [];
	let summary: LeetcodeEditorialSection | undefined;
	let recognition: LeetcodeEditorialSection | undefined;
	let keyIdea: LeetcodeEditorialSection | undefined;
	let reasoning: LeetcodeEditorialSection | undefined;
	let complexity: LeetcodeEditorialSection | undefined;
	let pitfalls: LeetcodeEditorialSection | undefined;
	let variants: LeetcodeEditorialSection | undefined;

	for (const section of sections) {
		if (!summary && SUMMARY_PATTERN.test(section.title)) {
			summary = section;
			continue;
		}
		if (!recognition && RECOGNITION_PATTERN.test(section.title)) {
			recognition = section;
			continue;
		}
		if (!keyIdea && KEY_IDEA_PATTERN.test(section.title)) {
			keyIdea = section;
			continue;
		}
		if (!reasoning && REASONING_PATTERN.test(section.title)) {
			reasoning = section;
			continue;
		}
		if (!complexity && COMPLEXITY_PATTERN.test(section.title)) {
			complexity = section;
			continue;
		}
		if (!pitfalls && PITFALLS_PATTERN.test(section.title)) {
			pitfalls = section;
			continue;
		}
		if (!variants && VARIANTS_PATTERN.test(section.title)) {
			variants = section;
			continue;
		}

		extras = [...extras, section];
	}

	return {
		summary,
		recognition,
		keyIdea,
		reasoning,
		complexity,
		pitfalls,
		variants,
		extras,
	};
}
