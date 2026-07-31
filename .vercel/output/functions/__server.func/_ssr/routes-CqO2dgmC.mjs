import { M as require_jsx_runtime, h as Link } from "../_libs/@tanstack/react-router+[...].mjs";
import { t as Button } from "./button-Cx_JF1io.mjs";
import { a as MemoryStick, c as ExternalLink, h as ArrowRight, l as Download, m as Binary, n as Sparkles, o as Github, p as BookOpen, r as ShieldCheck, s as GitBranch, t as Zap, u as Cpu } from "../_libs/lucide-react.mjs";
//#region node_modules/.nitro/vite/services/ssr/assets/routes-CqO2dgmC.js
var import_jsx_runtime = require_jsx_runtime();
var features = [
	{
		icon: Zap,
		title: "Adaptive routing",
		body: "Low-overhead probe samples the input and routes to the right path — no hand-tuning per dataset."
	},
	{
		icon: MemoryStick,
		title: "O(1) extra memory",
		body: "Fixed-size run tracking only. Adversarial many-run inputs fall back to pdqsort without heap growth."
	},
	{
		icon: Binary,
		title: "Matches ska on random ints",
		body: "Default less<> on scalar random data selects ska_sort (American-flag / MSD radix)."
	},
	{
		icon: GitBranch,
		title: "Strong on patterned data",
		body: "Verge-style run detection reverses descending runs, then finishes with pattern-defeating quicksort."
	},
	{
		icon: ShieldCheck,
		title: "Custom comparators",
		body: "Full Compare support through probe, reverse, and patterned paths. Early exits for sorted and reverse."
	},
	{
		icon: Cpu,
		title: "Header-only C++20",
		body: "Drop GeblomiSort.hpp into your include path. Concepts-constrained random-access iterators."
	}
];
var routes = [
	{
		name: "Small",
		desc: "n < 64 → pdqsort"
	},
	{
		name: "Sorted",
		desc: "Already ordered → no-op"
	},
	{
		name: "Reverse",
		desc: "Fully reverse → one reverse (+ re-sort if custom compare)"
	},
	{
		name: "Patterned",
		desc: "Low inversions / long runs → Verge-style + pdqsort"
	},
	{
		name: "Random",
		desc: "Scalars + less<> → ska_sort; else pdqsort"
	}
];
function HomePage() {
	return /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
		className: "min-h-dvh",
		children: [
			/* @__PURE__ */ (0, import_jsx_runtime.jsx)("header", {
				className: "sticky top-0 z-40 border-b border-border/80 bg-bg/90 backdrop-blur-md",
				children: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
					className: "mx-auto flex h-14 max-w-5xl items-center justify-between px-4 sm:px-6",
					children: [/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
						className: "flex items-center gap-2.5",
						children: [
							/* @__PURE__ */ (0, import_jsx_runtime.jsx)("div", {
								className: "flex h-8 w-8 items-center justify-center rounded-[var(--radius-sm)] border border-border-strong bg-bg-elevated font-mono text-xs font-semibold tracking-tight text-fg",
								children: "G"
							}),
							/* @__PURE__ */ (0, import_jsx_runtime.jsx)("span", {
								className: "text-sm font-semibold tracking-tight",
								children: "GeblomiSort"
							}),
							/* @__PURE__ */ (0, import_jsx_runtime.jsx)("span", {
								className: "hidden rounded-full border border-border px-2 py-0.5 font-mono text-[10px] text-fg-subtle sm:inline",
								children: "v2.5"
							})
						]
					}), /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("nav", {
						className: "flex items-center gap-1 sm:gap-2",
						children: [/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("a", {
							href: "https://x.com/HeywoodGeblomi",
							target: "_blank",
							rel: "noopener noreferrer me",
							className: "inline-flex h-10 items-center gap-1.5 rounded-[var(--radius-sm)] px-2.5 text-sm text-fg-muted transition-colors hover:bg-bg-elevated hover:text-fg",
							children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)(XIcon, { className: "h-4 w-4" }), /* @__PURE__ */ (0, import_jsx_runtime.jsx)("span", {
								className: "hidden sm:inline",
								children: "@HeywoodGeblomi"
							})]
						}), /* @__PURE__ */ (0, import_jsx_runtime.jsxs)(Link, {
							to: "/publish",
							className: "inline-flex h-10 items-center gap-1.5 rounded-[var(--radius-sm)] px-2.5 text-sm text-fg-muted transition-colors hover:bg-bg-elevated hover:text-fg",
							children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)(Github, { className: "h-4 w-4" }), /* @__PURE__ */ (0, import_jsx_runtime.jsx)("span", {
								className: "hidden sm:inline",
								children: "Publish"
							})]
						})]
					})]
				})
			}),
			/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("main", { children: [
				/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("section", {
					className: "relative overflow-hidden border-b border-border",
					children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)("div", {
						"aria-hidden": true,
						className: "pointer-events-none absolute inset-0 opacity-[0.35]",
						style: { background: "radial-gradient(ellipse 80% 50% at 50% -10%, color-mix(in oklab, var(--color-fg) 8%, transparent), transparent 70%)" }
					}), /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
						className: "relative mx-auto max-w-5xl px-4 py-16 sm:px-6 sm:py-24",
						children: [
							/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
								className: "mb-5 inline-flex items-center gap-2 rounded-full border border-border bg-bg-elevated px-3 py-1 text-xs text-fg-muted",
								children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)(Sparkles, { className: "h-3.5 w-3.5 text-fg-subtle" }), "Official open-source release kit"]
							}),
							/* @__PURE__ */ (0, import_jsx_runtime.jsx)("h1", {
								className: "max-w-3xl text-balance text-[clamp(2rem,5vw,3.25rem)] font-semibold leading-[1.1] tracking-[-0.03em] text-fg",
								children: "Adaptive hybrid sorting for C++20"
							}),
							/* @__PURE__ */ (0, import_jsx_runtime.jsx)("p", {
								className: "mt-5 max-w-2xl text-pretty text-base leading-relaxed text-fg-muted sm:text-lg",
								children: "GeblomiSort probes your data, then routes to early exits, Verge-style run handling, pdqsort, or ska_sort — with O(1) extra memory and full custom comparator support."
							}),
							/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
								className: "mt-8 flex flex-wrap items-center gap-3",
								children: [
									/* @__PURE__ */ (0, import_jsx_runtime.jsx)("a", {
										href: "/geblomi-sort/GeblomiSort.hpp",
										download: true,
										children: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)(Button, {
											size: "lg",
											children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)(Download, { className: "h-4 w-4" }), "Download header"]
										})
									}),
									/* @__PURE__ */ (0, import_jsx_runtime.jsx)(Link, {
										to: "/publish",
										children: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)(Button, {
											size: "lg",
											variant: "secondary",
											children: [
												/* @__PURE__ */ (0, import_jsx_runtime.jsx)(Github, { className: "h-4 w-4" }),
												"Push to GitHub",
												/* @__PURE__ */ (0, import_jsx_runtime.jsx)(ArrowRight, { className: "h-4 w-4" })
											]
										})
									}),
									/* @__PURE__ */ (0, import_jsx_runtime.jsx)("a", {
										href: "https://x.com/HeywoodGeblomi",
										target: "_blank",
										rel: "noopener noreferrer me",
										children: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)(Button, {
											size: "lg",
											variant: "outline",
											children: [
												/* @__PURE__ */ (0, import_jsx_runtime.jsx)(XIcon, { className: "h-4 w-4" }),
												"@HeywoodGeblomi",
												/* @__PURE__ */ (0, import_jsx_runtime.jsx)(ExternalLink, { className: "h-3.5 w-3.5 opacity-60" })
											]
										})
									})
								]
							}),
							/* @__PURE__ */ (0, import_jsx_runtime.jsx)("p", {
								className: "mt-6 font-mono text-xs text-fg-subtle",
								children: "Team: Grok + Harper + Benjamin + Lucas + Heywood · 2026-07-30"
							})
						]
					})]
				}),
				/* @__PURE__ */ (0, import_jsx_runtime.jsx)("section", {
					className: "border-b border-border",
					children: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
						className: "mx-auto max-w-5xl px-4 py-14 sm:px-6 sm:py-16",
						children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)("h2", {
							className: "text-sm font-medium uppercase tracking-[0.12em] text-fg-subtle",
							children: "Features"
						}), /* @__PURE__ */ (0, import_jsx_runtime.jsx)("div", {
							className: "mt-8 grid gap-4 sm:grid-cols-2 lg:grid-cols-3",
							children: features.map((f) => /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("article", {
								className: "rounded-[var(--radius-lg)] border border-border bg-bg-elevated p-5",
								children: [
									/* @__PURE__ */ (0, import_jsx_runtime.jsx)("div", {
										className: "mb-3 flex h-9 w-9 items-center justify-center rounded-[var(--radius-sm)] border border-border bg-bg-subtle",
										children: /* @__PURE__ */ (0, import_jsx_runtime.jsx)(f.icon, { className: "h-4 w-4 text-fg-muted" })
									}),
									/* @__PURE__ */ (0, import_jsx_runtime.jsx)("h3", {
										className: "text-sm font-semibold tracking-tight text-fg",
										children: f.title
									}),
									/* @__PURE__ */ (0, import_jsx_runtime.jsx)("p", {
										className: "mt-2 text-sm leading-relaxed text-fg-muted",
										children: f.body
									})
								]
							}, f.title))
						})]
					})
				}),
				/* @__PURE__ */ (0, import_jsx_runtime.jsx)("section", {
					className: "border-b border-border",
					children: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
						className: "mx-auto max-w-5xl px-4 py-14 sm:px-6 sm:py-16",
						children: [
							/* @__PURE__ */ (0, import_jsx_runtime.jsx)("h2", {
								className: "text-sm font-medium uppercase tracking-[0.12em] text-fg-subtle",
								children: "Adaptive routes"
							}),
							/* @__PURE__ */ (0, import_jsx_runtime.jsx)("p", {
								className: "mt-3 max-w-xl text-sm text-fg-muted",
								children: "The probe classifies the range, then the main entry switches on the result. No dynamic allocation in the hybrid path."
							}),
							/* @__PURE__ */ (0, import_jsx_runtime.jsx)("div", {
								className: "mt-8 overflow-hidden rounded-[var(--radius-lg)] border border-border",
								children: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("table", {
									className: "w-full text-left text-sm",
									children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)("thead", {
										className: "bg-bg-elevated text-fg-subtle",
										children: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("tr", { children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)("th", {
											className: "px-4 py-3 font-medium",
											children: "Route"
										}), /* @__PURE__ */ (0, import_jsx_runtime.jsx)("th", {
											className: "px-4 py-3 font-medium",
											children: "Behavior"
										})] })
									}), /* @__PURE__ */ (0, import_jsx_runtime.jsx)("tbody", { children: routes.map((r, i) => /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("tr", {
										className: i % 2 === 0 ? "bg-bg" : "bg-bg-elevated/50",
										children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)("td", {
											className: "px-4 py-3 font-mono text-xs font-medium text-fg",
											children: r.name
										}), /* @__PURE__ */ (0, import_jsx_runtime.jsx)("td", {
											className: "px-4 py-3 text-fg-muted",
											children: r.desc
										})]
									}, r.name)) })]
								})
							})
						]
					})
				}),
				/* @__PURE__ */ (0, import_jsx_runtime.jsx)("section", {
					className: "border-b border-border",
					children: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
						className: "mx-auto max-w-5xl px-4 py-14 sm:px-6 sm:py-16",
						children: [
							/* @__PURE__ */ (0, import_jsx_runtime.jsx)("h2", {
								className: "text-sm font-medium uppercase tracking-[0.12em] text-fg-subtle",
								children: "Usage"
							}),
							/* @__PURE__ */ (0, import_jsx_runtime.jsx)("div", {
								className: "mt-6 overflow-x-auto rounded-[var(--radius-lg)] border border-border bg-bg-elevated p-5",
								children: /* @__PURE__ */ (0, import_jsx_runtime.jsx)("pre", {
									className: "font-mono text-xs leading-relaxed text-fg sm:text-sm",
									children: /* @__PURE__ */ (0, import_jsx_runtime.jsx)("code", { children: `#include "GeblomiSort.hpp"

std::vector<int> v = /* ... */;
geblomi::sort(v.begin(), v.end());
geblomi::sort(v.begin(), v.end(), std::greater<>{});
geblomi::sort(v);  // container overload` })
								})
							}),
							/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("p", {
								className: "mt-4 text-sm text-fg-muted",
								children: [
									"Requirements: C++20, random-access iterators. Compile with",
									" ",
									/* @__PURE__ */ (0, import_jsx_runtime.jsx)("code", {
										className: "rounded bg-bg-subtle px-1.5 py-0.5 font-mono text-xs text-fg",
										children: "-std=c++20 -O3"
									}),
									"."
								]
							})
						]
					})
				}),
				/* @__PURE__ */ (0, import_jsx_runtime.jsx)("section", {
					className: "border-b border-border",
					children: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
						className: "mx-auto max-w-5xl px-4 py-14 sm:px-6 sm:py-16",
						children: [
							/* @__PURE__ */ (0, import_jsx_runtime.jsx)("h2", {
								className: "text-sm font-medium uppercase tracking-[0.12em] text-fg-subtle",
								children: "Package contents"
							}),
							/* @__PURE__ */ (0, import_jsx_runtime.jsx)("ul", {
								className: "mt-6 space-y-2 font-mono text-sm text-fg-muted",
								children: [
									"GeblomiSort.hpp — amalgamated header (pdqsort + ska_sort + hybrid)",
									"README.md — features, usage, credits",
									"LICENSE — MIT hybrid + retained third-party notices",
									"examples/demo.cpp — 1M-element correctness & timing demo",
									".gitignore"
								].map((line) => /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("li", {
									className: "flex gap-2 rounded-[var(--radius-sm)] border border-border bg-bg-elevated px-3 py-2.5",
									children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)(BookOpen, { className: "mt-0.5 h-3.5 w-3.5 shrink-0 text-fg-subtle" }), /* @__PURE__ */ (0, import_jsx_runtime.jsx)("span", { children: line })]
								}, line))
							}),
							/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
								className: "mt-10 rounded-[var(--radius-lg)] border border-border bg-bg-elevated p-5 sm:p-6",
								children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)("h3", {
									className: "text-sm font-semibold text-fg",
									children: "Credits & licenses"
								}), /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("ul", {
									className: "mt-4 space-y-3 text-sm text-fg-muted",
									children: [
										/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("li", { children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)("span", {
											className: "font-medium text-fg",
											children: "pdqsort"
										}), " — Orson Peters (2021), zlib-style (notice retained in header)"] }),
										/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("li", { children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)("span", {
											className: "font-medium text-fg",
											children: "ska_sort"
										}), " — Malte Skarupke (2016), Boost Software License 1.0"] }),
										/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("li", { children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)("span", {
											className: "font-medium text-fg",
											children: "Geblomi hybrid"
										}), " — Grok + Harper + Benjamin + Lucas + Heywood, MIT"] })
									]
								})]
							})
						]
					})
				}),
				/* @__PURE__ */ (0, import_jsx_runtime.jsx)("section", { children: /* @__PURE__ */ (0, import_jsx_runtime.jsx)("div", {
					className: "mx-auto max-w-5xl px-4 py-14 sm:px-6 sm:py-20",
					children: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
						className: "rounded-[var(--radius-xl)] border border-border-strong bg-bg-elevated p-6 sm:p-10",
						children: [
							/* @__PURE__ */ (0, import_jsx_runtime.jsx)("h2", {
								className: "text-xl font-semibold tracking-tight text-fg sm:text-2xl",
								children: "Ready for official publish"
							}),
							/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("p", {
								className: "mt-3 max-w-xl text-sm leading-relaxed text-fg-muted sm:text-base",
								children: [
									"Follow the guided steps to create the public GitHub repo, push this package, link your profile to",
									" ",
									/* @__PURE__ */ (0, import_jsx_runtime.jsx)("a", {
										href: "https://x.com/HeywoodGeblomi",
										className: "text-fg underline decoration-border-strong underline-offset-4 hover:decoration-fg",
										target: "_blank",
										rel: "noopener noreferrer me",
										children: "@HeywoodGeblomi"
									}),
									", and announce the release."
								]
							}),
							/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
								className: "mt-6 flex flex-wrap gap-3",
								children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)(Link, {
									to: "/publish",
									children: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)(Button, {
										size: "lg",
										children: ["Open publish guide", /* @__PURE__ */ (0, import_jsx_runtime.jsx)(ArrowRight, { className: "h-4 w-4" })]
									})
								}), /* @__PURE__ */ (0, import_jsx_runtime.jsx)("a", {
									href: "/geblomi-sort/README.md",
									download: true,
									children: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)(Button, {
										size: "lg",
										variant: "secondary",
										children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)(Download, { className: "h-4 w-4" }), "README.md"]
									})
								})]
							})
						]
					})
				}) })
			] }),
			/* @__PURE__ */ (0, import_jsx_runtime.jsx)("footer", {
				className: "border-t border-border",
				children: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
					className: "mx-auto flex max-w-5xl flex-col gap-3 px-4 py-8 text-xs text-fg-subtle sm:flex-row sm:items-center sm:justify-between sm:px-6",
					children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)("p", { children: "GeblomiSort v2.5 · Buffer-safe adaptive hybrid" }), /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("a", {
						href: "https://x.com/HeywoodGeblomi",
						target: "_blank",
						rel: "noopener noreferrer me",
						className: "inline-flex items-center gap-1.5 text-fg-muted transition-colors hover:text-fg",
						children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)(XIcon, { className: "h-3.5 w-3.5" }), "@HeywoodGeblomi"]
					})]
				})
			})
		]
	});
}
function XIcon({ className }) {
	return /* @__PURE__ */ (0, import_jsx_runtime.jsx)("svg", {
		viewBox: "0 0 24 24",
		fill: "currentColor",
		className,
		"aria-hidden": true,
		children: /* @__PURE__ */ (0, import_jsx_runtime.jsx)("path", { d: "M18.244 2.25h3.308l-7.227 8.26 8.502 11.24H16.17l-4.714-6.231-5.401 6.231H2.744l7.727-8.835L1.254 2.25H8.08l4.253 5.622L18.244 2.25zm-1.161 17.52h1.833L7.084 4.126H5.117L17.083 19.77z" })
	});
}
//#endregion
export { HomePage as component };
