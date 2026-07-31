import { createFileRoute, Link } from "@tanstack/react-router";
import {
  ArrowLeft,
  CheckCircle2,
  Copy,
  Download,
  ExternalLink,
  Github,
  Package,
} from "lucide-react";
import { useState } from "react";
import { Button } from "@/components/ui/button";

export const Route = createFileRoute("/publish")({
  component: PublishPage,
});

const packageFiles = [
  {
    name: "GeblomiSort-v2.5.tar.gz",
    href: "/GeblomiSort-v2.5.tar.gz",
    note: "Full package (recommended)",
  },
  { name: "GeblomiSort.hpp", href: "/geblomi-sort/GeblomiSort.hpp", note: "Main header" },
  { name: "README.md", href: "/geblomi-sort/README.md", note: "Docs + X badge" },
  { name: "LICENSE", href: "/geblomi-sort/LICENSE", note: "MIT hybrid + third-party" },
  { name: "examples/demo.cpp", href: "/geblomi-sort/examples/demo.cpp", note: "Demo" },
  { name: ".gitignore", href: "/geblomi-sort/.gitignore", note: "Ignore rules" },
];

const steps = [
  {
    title: "Create the GitHub repository",
    body: (
      <>
        Log in to GitHub and create a <strong className="text-fg">public</strong> repo
        named <code className="font-mono text-xs text-fg">GeblomiSort</code>. Do not
        initialize with a README (we already have one). Suggested path:{" "}
        <code className="font-mono text-xs text-fg">YOUR_USER/GeblomiSort</code>.
      </>
    ),
  },
  {
    title: "Download the package",
    body: (
      <>
        Prefer the tarball for a complete tree. Or download files individually. Layout:{" "}
        header, README, LICENSE, and .gitignore at the repo root;{" "}
        <code className="font-mono text-xs text-fg">examples/demo.cpp</code> nested.
      </>
    ),
  },
  {
    title: "Push with git",
    body: (
      <>
        Clone the empty repo, unpack or copy the package files in, then commit and
        push. Commands are ready to copy in the panel on the right — replace{" "}
        <code className="font-mono text-xs text-fg">YOUR_USERNAME</code>.
      </>
    ),
  },
  {
    title: "Link X on GitHub",
    body: (
      <>
        GitHub → Settings → Public profile → Social accounts → add{" "}
        <a
          href="https://x.com/HeywoodGeblomi"
          className="text-fg underline decoration-border-strong underline-offset-4"
          target="_blank"
          rel="noopener noreferrer me"
        >
          x.com/HeywoodGeblomi
        </a>
        . The README already badges and links your handle.
      </>
    ),
  },
  {
    title: "Polish the repo",
    body: (
      <>
        Add topics:{" "}
        <code className="font-mono text-xs text-fg">
          cpp, sorting, header-only, algorithms, pdqsort, radix-sort, c-plus-plus-20
        </code>
        . Create release tag <code className="font-mono text-xs text-fg">v2.5.0</code>{" "}
        and attach the tarball.
      </>
    ),
  },
  {
    title: "Announce on X",
    body: (
      <>
        Post from{" "}
        <a
          href="https://x.com/HeywoodGeblomi"
          className="text-fg underline decoration-border-strong underline-offset-4"
          target="_blank"
          rel="noopener noreferrer me"
        >
          @HeywoodGeblomi
        </a>{" "}
        with the repo URL. A draft is below — paste and swap in your username.
      </>
    ),
  },
];

const gitCommands = `git clone https://github.com/YOUR_USERNAME/GeblomiSort.git
cd GeblomiSort
# Option A: unpack tarball
# tar -xzf GeblomiSort-v2.5.tar.gz
# Option B: copy GeblomiSort.hpp README.md LICENSE .gitignore examples/ here
git add .
git commit -m "GeblomiSort v2.5 — adaptive hybrid (probe + Verge-style + pdqsort/ska_sort)"
git branch -M main
git push -u origin main`;

const xDraft = `Just published GeblomiSort — adaptive hybrid C++20 sorting algorithm (header-only).

Low-overhead probing + Verge-style runs for patterned data, smart routing to pdqsort / ska_sort.

• O(1) extra memory
• Full custom comparators
• Matches ska_sort on pure random ints, strong on nearly-sorted/patterned
• Early exits for sorted/reverse

Repo: https://github.com/YOUR_USERNAME/GeblomiSort

#cpp #algorithms #opensource`;

function PublishPage() {
  const [copied, setCopied] = useState<"git" | "x" | null>(null);

  async function copy(text: string, key: "git" | "x") {
    try {
      await navigator.clipboard.writeText(text);
      setCopied(key);
      window.setTimeout(() => setCopied(null), 2000);
    } catch {
      /* ignore */
    }
  }

  return (
    <div className="min-h-dvh">
      <header className="sticky top-0 z-40 border-b border-border/80 bg-bg/90 backdrop-blur-md">
        <div className="mx-auto flex h-14 max-w-5xl items-center justify-between px-4 sm:px-6">
          <Link
            to="/"
            className="inline-flex h-10 items-center gap-2 text-sm text-fg-muted transition-colors hover:text-fg"
          >
            <ArrowLeft className="h-4 w-4" />
            GeblomiSort
          </Link>
          <a
            href="https://x.com/HeywoodGeblomi"
            target="_blank"
            rel="noopener noreferrer me"
            className="inline-flex h-10 items-center gap-1.5 rounded-[var(--radius-sm)] px-2.5 text-sm text-fg-muted transition-colors hover:bg-bg-elevated hover:text-fg"
          >
            <XIcon className="h-4 w-4" />
            @HeywoodGeblomi
          </a>
        </div>
      </header>

      <main className="mx-auto max-w-5xl px-4 py-10 sm:px-6 sm:py-14">
        <div className="mb-2 inline-flex items-center gap-2 text-xs font-medium uppercase tracking-[0.12em] text-fg-subtle">
          <Github className="h-3.5 w-3.5" />
          Official publish
        </div>
        <h1 className="text-balance text-3xl font-semibold tracking-[-0.03em] text-fg sm:text-4xl">
          Push GeblomiSort to GitHub
        </h1>
        <p className="mt-4 max-w-2xl text-sm leading-relaxed text-fg-muted sm:text-base">
          I cannot log into your GitHub account from here. Everything you need is
          prepared below — download the files, run the git commands on your machine,
          and link{" "}
          <a
            href="https://x.com/HeywoodGeblomi"
            className="text-fg underline decoration-border-strong underline-offset-4"
            target="_blank"
            rel="noopener noreferrer me"
          >
            @HeywoodGeblomi
          </a>
          .
        </p>

        <div className="mt-10 grid gap-8 lg:grid-cols-[1fr_minmax(0,22rem)]">
          <ol className="space-y-4">
            {steps.map((step, i) => (
              <li
                key={step.title}
                className="rounded-[var(--radius-lg)] border border-border bg-bg-elevated p-5"
              >
                <div className="flex items-start gap-3">
                  <span className="flex h-7 w-7 shrink-0 items-center justify-center rounded-full border border-border-strong bg-bg font-mono text-xs font-semibold text-fg">
                    {i + 1}
                  </span>
                  <div>
                    <h2 className="text-sm font-semibold text-fg">{step.title}</h2>
                    <div className="mt-2 text-sm leading-relaxed text-fg-muted">
                      {step.body}
                    </div>
                  </div>
                </div>
              </li>
            ))}
          </ol>

          <aside className="space-y-4 lg:sticky lg:top-20 lg:self-start">
            <div className="rounded-[var(--radius-lg)] border border-border bg-bg-elevated p-5">
              <div className="mb-3 flex items-center gap-2 text-sm font-semibold text-fg">
                <Package className="h-4 w-4 text-fg-muted" />
                Package files
              </div>
              <ul className="space-y-2">
                {packageFiles.map((f) => (
                  <li key={f.name}>
                    <a
                      href={f.href}
                      download
                      className="flex items-center justify-between gap-2 rounded-[var(--radius-sm)] border border-border bg-bg px-3 py-2.5 text-sm transition-colors hover:border-border-strong hover:bg-bg-subtle"
                    >
                      <span className="min-w-0">
                        <span className="block truncate font-mono text-xs text-fg">
                          {f.name}
                        </span>
                        <span className="text-[11px] text-fg-subtle">{f.note}</span>
                      </span>
                      <Download className="h-3.5 w-3.5 shrink-0 text-fg-subtle" />
                    </a>
                  </li>
                ))}
              </ul>
            </div>

            <div className="rounded-[var(--radius-lg)] border border-border bg-bg-elevated p-5">
              <div className="mb-2 flex items-center justify-between gap-2">
                <h3 className="text-sm font-semibold text-fg">Git commands</h3>
                <Button
                  size="sm"
                  variant="secondary"
                  onClick={() => copy(gitCommands, "git")}
                  type="button"
                >
                  {copied === "git" ? (
                    <CheckCircle2 className="h-3.5 w-3.5 text-success" />
                  ) : (
                    <Copy className="h-3.5 w-3.5" />
                  )}
                  {copied === "git" ? "Copied" : "Copy"}
                </Button>
              </div>
              <pre className="overflow-x-auto rounded-[var(--radius-sm)] border border-border bg-bg p-3 font-mono text-[11px] leading-relaxed text-fg-muted">
                {gitCommands}
              </pre>
            </div>

            <div className="rounded-[var(--radius-lg)] border border-border bg-bg-elevated p-5">
              <div className="mb-2 flex items-center justify-between gap-2">
                <h3 className="text-sm font-semibold text-fg">X announcement</h3>
                <Button
                  size="sm"
                  variant="secondary"
                  onClick={() => copy(xDraft, "x")}
                  type="button"
                >
                  {copied === "x" ? (
                    <CheckCircle2 className="h-3.5 w-3.5 text-success" />
                  ) : (
                    <Copy className="h-3.5 w-3.5" />
                  )}
                  {copied === "x" ? "Copied" : "Copy"}
                </Button>
              </div>
              <pre className="whitespace-pre-wrap rounded-[var(--radius-sm)] border border-border bg-bg p-3 font-mono text-[11px] leading-relaxed text-fg-muted">
                {xDraft}
              </pre>
              <a
                href="https://x.com/intent/post"
                target="_blank"
                rel="noopener noreferrer"
                className="mt-3 inline-flex items-center gap-1.5 text-xs text-fg-muted transition-colors hover:text-fg"
              >
                Open X compose
                <ExternalLink className="h-3 w-3" />
              </a>
            </div>
          </aside>
        </div>
      </main>
    </div>
  );
}

function XIcon({ className }: { className?: string }) {
  return (
    <svg
      viewBox="0 0 24 24"
      fill="currentColor"
      className={className}
      aria-hidden
    >
      <path d="M18.244 2.25h3.308l-7.227 8.26 8.502 11.24H16.17l-4.714-6.231-5.401 6.231H2.744l7.727-8.835L1.254 2.25H8.08l4.253 5.622L18.244 2.25zm-1.161 17.52h1.833L7.084 4.126H5.117L17.083 19.77z" />
    </svg>
  );
}
