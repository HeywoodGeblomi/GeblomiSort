import { r as __toESM } from "../_runtime.mjs";
import { M as require_jsx_runtime, N as require_react, h as Link } from "../_libs/@tanstack/react-router+[...].mjs";
import { t as Button } from "./button-Cx_JF1io.mjs";
import { c as ExternalLink, d as Copy, f as CircleCheck, g as ArrowLeft, i as Package, l as Download, o as Github } from "../_libs/lucide-react.mjs";
//#region node_modules/.nitro/vite/services/ssr/assets/publish-C7k5H4P9.js
var import_react = /* @__PURE__ */ __toESM(require_react());
var import_jsx_runtime = require_jsx_runtime();
var packageFiles = [
	{
		name: "GeblomiSort-v2.5.tar.gz",
		href: "/GeblomiSort-v2.5.tar.gz",
		note: "Full package (recommended)"
	},
	{
		name: "GeblomiSort.hpp",
		href: "/geblomi-sort/GeblomiSort.hpp",
		note: "Main header"
	},
	{
		name: "README.md",
		href: "/geblomi-sort/README.md",
		note: "Docs + X badge"
	},
	{
		name: "LICENSE",
		href: "/geblomi-sort/LICENSE",
		note: "MIT hybrid + third-party"
	},
	{
		name: "examples/demo.cpp",
		href: "/geblomi-sort/examples/demo.cpp",
		note: "Demo"
	},
	{
		name: ".gitignore",
		href: "/geblomi-sort/.gitignore",
		note: "Ignore rules"
	}
];
var steps = [
	{
		title: "Create the GitHub repository",
		body: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)(import_jsx_runtime.Fragment, { children: [
			"Log in to GitHub and create a ",
			/* @__PURE__ */ (0, import_jsx_runtime.jsx)("strong", {
				className: "text-fg",
				children: "public"
			}),
			" repo named ",
			/* @__PURE__ */ (0, import_jsx_runtime.jsx)("code", {
				className: "font-mono text-xs text-fg",
				children: "GeblomiSort"
			}),
			". Do not initialize with a README (we already have one). Suggested path:",
			" ",
			/* @__PURE__ */ (0, import_jsx_runtime.jsx)("code", {
				className: "font-mono text-xs text-fg",
				children: "YOUR_USER/GeblomiSort"
			}),
			"."
		] })
	},
	{
		title: "Download the package",
		body: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)(import_jsx_runtime.Fragment, { children: [
			"Prefer the tarball for a complete tree. Or download files individually. Layout:",
			" ",
			"header, README, LICENSE, and .gitignore at the repo root;",
			" ",
			/* @__PURE__ */ (0, import_jsx_runtime.jsx)("code", {
				className: "font-mono text-xs text-fg",
				children: "examples/demo.cpp"
			}),
			" nested."
		] })
	},
	{
		title: "Push with git",
		body: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)(import_jsx_runtime.Fragment, { children: [
			"Clone the empty repo, unpack or copy the package files in, then commit and push. Commands are ready to copy in the panel on the right — replace",
			" ",
			/* @__PURE__ */ (0, import_jsx_runtime.jsx)("code", {
				className: "font-mono text-xs text-fg",
				children: "YOUR_USERNAME"
			}),
			"."
		] })
	},
	{
		title: "Link X on GitHub",
		body: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)(import_jsx_runtime.Fragment, { children: [
			"GitHub → Settings → Public profile → Social accounts → add",
			" ",
			/* @__PURE__ */ (0, import_jsx_runtime.jsx)("a", {
				href: "https://x.com/HeywoodGeblomi",
				className: "text-fg underline decoration-border-strong underline-offset-4",
				target: "_blank",
				rel: "noopener noreferrer me",
				children: "x.com/HeywoodGeblomi"
			}),
			". The README already badges and links your handle."
		] })
	},
	{
		title: "Polish the repo",
		body: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)(import_jsx_runtime.Fragment, { children: [
			"Add topics:",
			" ",
			/* @__PURE__ */ (0, import_jsx_runtime.jsx)("code", {
				className: "font-mono text-xs text-fg",
				children: "cpp, sorting, header-only, algorithms, pdqsort, radix-sort, c-plus-plus-20"
			}),
			". Create release tag ",
			/* @__PURE__ */ (0, import_jsx_runtime.jsx)("code", {
				className: "font-mono text-xs text-fg",
				children: "v2.5.0"
			}),
			" ",
			"and attach the tarball."
		] })
	},
	{
		title: "Announce on X",
		body: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)(import_jsx_runtime.Fragment, { children: [
			"Post from",
			" ",
			/* @__PURE__ */ (0, import_jsx_runtime.jsx)("a", {
				href: "https://x.com/HeywoodGeblomi",
				className: "text-fg underline decoration-border-strong underline-offset-4",
				target: "_blank",
				rel: "noopener noreferrer me",
				children: "@HeywoodGeblomi"
			}),
			" ",
			"with the repo URL. A draft is below — paste and swap in your username."
		] })
	}
];
var gitCommands = `git clone https://github.com/YOUR_USERNAME/GeblomiSort.git
cd GeblomiSort
# Option A: unpack tarball
# tar -xzf GeblomiSort-v2.5.tar.gz
# Option B: copy GeblomiSort.hpp README.md LICENSE .gitignore examples/ here
git add .
git commit -m "GeblomiSort v2.5 — adaptive hybrid (probe + Verge-style + pdqsort/ska_sort)"
git branch -M main
git push -u origin main`;
var xDraft = `Just published GeblomiSort — adaptive hybrid C++20 sorting algorithm (header-only).

Low-overhead probing + Verge-style runs for patterned data, smart routing to pdqsort / ska_sort.

• O(1) extra memory
• Full custom comparators
• Matches ska_sort on pure random ints, strong on nearly-sorted/patterned
• Early exits for sorted/reverse

Repo: https://github.com/YOUR_USERNAME/GeblomiSort

#cpp #algorithms #opensource`;
function PublishPage() {
	const [copied, setCopied] = (0, import_react.useState)(null);
	async function copy(text, key) {
		try {
			await navigator.clipboard.writeText(text);
			setCopied(key);
			window.setTimeout(() => setCopied(null), 2e3);
		} catch {}
	}
	return /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
		className: "min-h-dvh",
		children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)("header", {
			className: "sticky top-0 z-40 border-b border-border/80 bg-bg/90 backdrop-blur-md",
			children: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
				className: "mx-auto flex h-14 max-w-5xl items-center justify-between px-4 sm:px-6",
				children: [/* @__PURE__ */ (0, import_jsx_runtime.jsxs)(Link, {
					to: "/",
					className: "inline-flex h-10 items-center gap-2 text-sm text-fg-muted transition-colors hover:text-fg",
					children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)(ArrowLeft, { className: "h-4 w-4" }), "GeblomiSort"]
				}), /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("a", {
					href: "https://x.com/HeywoodGeblomi",
					target: "_blank",
					rel: "noopener noreferrer me",
					className: "inline-flex h-10 items-center gap-1.5 rounded-[var(--radius-sm)] px-2.5 text-sm text-fg-muted transition-colors hover:bg-bg-elevated hover:text-fg",
					children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)(XIcon, { className: "h-4 w-4" }), "@HeywoodGeblomi"]
				})]
			})
		}), /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("main", {
			className: "mx-auto max-w-5xl px-4 py-10 sm:px-6 sm:py-14",
			children: [
				/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
					className: "mb-2 inline-flex items-center gap-2 text-xs font-medium uppercase tracking-[0.12em] text-fg-subtle",
					children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)(Github, { className: "h-3.5 w-3.5" }), "Official publish"]
				}),
				/* @__PURE__ */ (0, import_jsx_runtime.jsx)("h1", {
					className: "text-balance text-3xl font-semibold tracking-[-0.03em] text-fg sm:text-4xl",
					children: "Push GeblomiSort to GitHub"
				}),
				/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("p", {
					className: "mt-4 max-w-2xl text-sm leading-relaxed text-fg-muted sm:text-base",
					children: [
						"I cannot log into your GitHub account from here. Everything you need is prepared below — download the files, run the git commands on your machine, and link",
						" ",
						/* @__PURE__ */ (0, import_jsx_runtime.jsx)("a", {
							href: "https://x.com/HeywoodGeblomi",
							className: "text-fg underline decoration-border-strong underline-offset-4",
							target: "_blank",
							rel: "noopener noreferrer me",
							children: "@HeywoodGeblomi"
						}),
						"."
					]
				}),
				/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
					className: "mt-10 grid gap-8 lg:grid-cols-[1fr_minmax(0,22rem)]",
					children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)("ol", {
						className: "space-y-4",
						children: steps.map((step, i) => /* @__PURE__ */ (0, import_jsx_runtime.jsx)("li", {
							className: "rounded-[var(--radius-lg)] border border-border bg-bg-elevated p-5",
							children: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
								className: "flex items-start gap-3",
								children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)("span", {
									className: "flex h-7 w-7 shrink-0 items-center justify-center rounded-full border border-border-strong bg-bg font-mono text-xs font-semibold text-fg",
									children: i + 1
								}), /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", { children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)("h2", {
									className: "text-sm font-semibold text-fg",
									children: step.title
								}), /* @__PURE__ */ (0, import_jsx_runtime.jsx)("div", {
									className: "mt-2 text-sm leading-relaxed text-fg-muted",
									children: step.body
								})] })]
							})
						}, step.title))
					}), /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("aside", {
						className: "space-y-4 lg:sticky lg:top-20 lg:self-start",
						children: [
							/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
								className: "rounded-[var(--radius-lg)] border border-border bg-bg-elevated p-5",
								children: [/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
									className: "mb-3 flex items-center gap-2 text-sm font-semibold text-fg",
									children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)(Package, { className: "h-4 w-4 text-fg-muted" }), "Package files"]
								}), /* @__PURE__ */ (0, import_jsx_runtime.jsx)("ul", {
									className: "space-y-2",
									children: packageFiles.map((f) => /* @__PURE__ */ (0, import_jsx_runtime.jsx)("li", { children: /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("a", {
										href: f.href,
										download: true,
										className: "flex items-center justify-between gap-2 rounded-[var(--radius-sm)] border border-border bg-bg px-3 py-2.5 text-sm transition-colors hover:border-border-strong hover:bg-bg-subtle",
										children: [/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("span", {
											className: "min-w-0",
											children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)("span", {
												className: "block truncate font-mono text-xs text-fg",
												children: f.name
											}), /* @__PURE__ */ (0, import_jsx_runtime.jsx)("span", {
												className: "text-[11px] text-fg-subtle",
												children: f.note
											})]
										}), /* @__PURE__ */ (0, import_jsx_runtime.jsx)(Download, { className: "h-3.5 w-3.5 shrink-0 text-fg-subtle" })]
									}) }, f.name))
								})]
							}),
							/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
								className: "rounded-[var(--radius-lg)] border border-border bg-bg-elevated p-5",
								children: [/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
									className: "mb-2 flex items-center justify-between gap-2",
									children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)("h3", {
										className: "text-sm font-semibold text-fg",
										children: "Git commands"
									}), /* @__PURE__ */ (0, import_jsx_runtime.jsxs)(Button, {
										size: "sm",
										variant: "secondary",
										onClick: () => copy(gitCommands, "git"),
										type: "button",
										children: [copied === "git" ? /* @__PURE__ */ (0, import_jsx_runtime.jsx)(CircleCheck, { className: "h-3.5 w-3.5 text-success" }) : /* @__PURE__ */ (0, import_jsx_runtime.jsx)(Copy, { className: "h-3.5 w-3.5" }), copied === "git" ? "Copied" : "Copy"]
									})]
								}), /* @__PURE__ */ (0, import_jsx_runtime.jsx)("pre", {
									className: "overflow-x-auto rounded-[var(--radius-sm)] border border-border bg-bg p-3 font-mono text-[11px] leading-relaxed text-fg-muted",
									children: gitCommands
								})]
							}),
							/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
								className: "rounded-[var(--radius-lg)] border border-border bg-bg-elevated p-5",
								children: [
									/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("div", {
										className: "mb-2 flex items-center justify-between gap-2",
										children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)("h3", {
											className: "text-sm font-semibold text-fg",
											children: "X announcement"
										}), /* @__PURE__ */ (0, import_jsx_runtime.jsxs)(Button, {
											size: "sm",
											variant: "secondary",
											onClick: () => copy(xDraft, "x"),
											type: "button",
											children: [copied === "x" ? /* @__PURE__ */ (0, import_jsx_runtime.jsx)(CircleCheck, { className: "h-3.5 w-3.5 text-success" }) : /* @__PURE__ */ (0, import_jsx_runtime.jsx)(Copy, { className: "h-3.5 w-3.5" }), copied === "x" ? "Copied" : "Copy"]
										})]
									}),
									/* @__PURE__ */ (0, import_jsx_runtime.jsx)("pre", {
										className: "whitespace-pre-wrap rounded-[var(--radius-sm)] border border-border bg-bg p-3 font-mono text-[11px] leading-relaxed text-fg-muted",
										children: xDraft
									}),
									/* @__PURE__ */ (0, import_jsx_runtime.jsxs)("a", {
										href: "https://x.com/intent/post",
										target: "_blank",
										rel: "noopener noreferrer",
										className: "mt-3 inline-flex items-center gap-1.5 text-xs text-fg-muted transition-colors hover:text-fg",
										children: ["Open X compose", /* @__PURE__ */ (0, import_jsx_runtime.jsx)(ExternalLink, { className: "h-3 w-3" })]
									})
								]
							})
						]
					})]
				})
			]
		})]
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
export { PublishPage as component };
