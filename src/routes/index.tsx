import { createFileRoute, Link } from "@tanstack/react-router";
import {
  ArrowRight,
  Binary,
  BookOpen,
  Cpu,
  Download,
  ExternalLink,
  GitBranch,
  Github,
  MemoryStick,
  ShieldCheck,
  Sparkles,
  Zap,
} from "lucide-react";
import { Button } from "@/components/ui/button";

export const Route = createFileRoute("/")({
  component: HomePage,
});

const features = [
  {
    icon: Zap,
    title: "Adaptive routing",
    body: "Low-overhead probe samples the input and routes to the right path — no hand-tuning per dataset.",
  },
  {
    icon: MemoryStick,
    title: "O(1) extra memory",
    body: "Fixed-size run tracking only. Adversarial many-run inputs fall back to pdqsort without heap growth.",
  },
  {
    icon: Binary,
    title: "Matches ska on random ints",
    body: "Default less<> on scalar random data selects ska_sort (American-flag / MSD radix).",
  },
  {
    icon: GitBranch,
    title: "Strong on patterned data",
    body: "Verge-style run detection reverses descending runs, then finishes with pattern-defeating quicksort.",
  },
  {
    icon: ShieldCheck,
    title: "Custom comparators",
    body: "Full Compare support through probe, reverse, and patterned paths. Early exits for sorted and reverse.",
  },
  {
    icon: Cpu,
    title: "Header-only C++20",
    body: "Drop GeblomiSort.hpp into your include path. Concepts-constrained random-access iterators.",
  },
];

const routes = [
  { name: "Small", desc: "n < 64 → pdqsort" },
  { name: "Sorted", desc: "Already ordered → no-op" },
  { name: "Reverse", desc: "Fully reverse → one reverse (+ re-sort if custom compare)" },
  { name: "Patterned", desc: "Low inversions / long runs → Verge-style + pdqsort" },
  { name: "Random", desc: "Scalars + less<> → ska_sort; else pdqsort" },
];

function HomePage() {
  return (
    <div className="min-h-dvh">
      <header className="sticky top-0 z-40 border-b border-border/80 bg-bg/90 backdrop-blur-md">
        <div className="mx-auto flex h-14 max-w-5xl items-center justify-between px-4 sm:px-6">
          <div className="flex items-center gap-2.5">
            <div className="flex h-8 w-8 items-center justify-center rounded-[var(--radius-sm)] border border-border-strong bg-bg-elevated font-mono text-xs font-semibold tracking-tight text-fg">
              G
            </div>
            <span className="text-sm font-semibold tracking-tight">GeblomiSort</span>
            <span className="hidden rounded-full border border-border px-2 py-0.5 font-mono text-[10px] text-fg-subtle sm:inline">
              v2.5
            </span>
          </div>
          <nav className="flex items-center gap-1 sm:gap-2">
            <a
              href="https://x.com/HeywoodGeblomi"
              target="_blank"
              rel="noopener noreferrer me"
              className="inline-flex h-10 items-center gap-1.5 rounded-[var(--radius-sm)] px-2.5 text-sm text-fg-muted transition-colors hover:bg-bg-elevated hover:text-fg"
            >
              <XIcon className="h-4 w-4" />
              <span className="hidden sm:inline">@HeywoodGeblomi</span>
            </a>
            <Link
              to="/publish"
              className="inline-flex h-10 items-center gap-1.5 rounded-[var(--radius-sm)] px-2.5 text-sm text-fg-muted transition-colors hover:bg-bg-elevated hover:text-fg"
            >
              <Github className="h-4 w-4" />
              <span className="hidden sm:inline">Publish</span>
            </Link>
          </nav>
        </div>
      </header>

      <main>
        {/* Hero */}
        <section className="relative overflow-hidden border-b border-border">
          <div
            aria-hidden
            className="pointer-events-none absolute inset-0 opacity-[0.35]"
            style={{
              background:
                "radial-gradient(ellipse 80% 50% at 50% -10%, color-mix(in oklab, var(--color-fg) 8%, transparent), transparent 70%)",
            }}
          />
          <div className="relative mx-auto max-w-5xl px-4 py-16 sm:px-6 sm:py-24">
            <div className="mb-5 inline-flex items-center gap-2 rounded-full border border-border bg-bg-elevated px-3 py-1 text-xs text-fg-muted">
              <Sparkles className="h-3.5 w-3.5 text-fg-subtle" />
              Official open-source release kit
            </div>
            <h1 className="max-w-3xl text-balance text-[clamp(2rem,5vw,3.25rem)] font-semibold leading-[1.1] tracking-[-0.03em] text-fg">
              Adaptive hybrid sorting for C++20
            </h1>
            <p className="mt-5 max-w-2xl text-pretty text-base leading-relaxed text-fg-muted sm:text-lg">
              GeblomiSort probes your data, then routes to early exits, Verge-style
              run handling, pdqsort, or ska_sort — with O(1) extra memory and full
              custom comparator support.
            </p>
            <div className="mt-8 flex flex-wrap items-center gap-3">
              <a href="/geblomi-sort/GeblomiSort.hpp" download>
                <Button size="lg">
                  <Download className="h-4 w-4" />
                  Download header
                </Button>
              </a>
              <Link to="/publish">
                <Button size="lg" variant="secondary">
                  <Github className="h-4 w-4" />
                  Push to GitHub
                  <ArrowRight className="h-4 w-4" />
                </Button>
              </Link>
              <a
                href="https://x.com/HeywoodGeblomi"
                target="_blank"
                rel="noopener noreferrer me"
              >
                <Button size="lg" variant="outline">
                  <XIcon className="h-4 w-4" />
                  @HeywoodGeblomi
                  <ExternalLink className="h-3.5 w-3.5 opacity-60" />
                </Button>
              </a>
            </div>
            <p className="mt-6 font-mono text-xs text-fg-subtle">
              Team: Grok + Harper + Benjamin + Lucas + Heywood · 2026-07-30
            </p>
          </div>
        </section>

        {/* Features */}
        <section className="border-b border-border">
          <div className="mx-auto max-w-5xl px-4 py-14 sm:px-6 sm:py-16">
            <h2 className="text-sm font-medium uppercase tracking-[0.12em] text-fg-subtle">
              Features
            </h2>
            <div className="mt-8 grid gap-4 sm:grid-cols-2 lg:grid-cols-3">
              {features.map((f) => (
                <article
                  key={f.title}
                  className="rounded-[var(--radius-lg)] border border-border bg-bg-elevated p-5"
                >
                  <div className="mb-3 flex h-9 w-9 items-center justify-center rounded-[var(--radius-sm)] border border-border bg-bg-subtle">
                    <f.icon className="h-4 w-4 text-fg-muted" />
                  </div>
                  <h3 className="text-sm font-semibold tracking-tight text-fg">{f.title}</h3>
                  <p className="mt-2 text-sm leading-relaxed text-fg-muted">{f.body}</p>
                </article>
              ))}
            </div>
          </div>
        </section>

        {/* Routing */}
        <section className="border-b border-border">
          <div className="mx-auto max-w-5xl px-4 py-14 sm:px-6 sm:py-16">
            <h2 className="text-sm font-medium uppercase tracking-[0.12em] text-fg-subtle">
              Adaptive routes
            </h2>
            <p className="mt-3 max-w-xl text-sm text-fg-muted">
              The probe classifies the range, then the main entry switches on the
              result. No dynamic allocation in the hybrid path.
            </p>
            <div className="mt-8 overflow-hidden rounded-[var(--radius-lg)] border border-border">
              <table className="w-full text-left text-sm">
                <thead className="bg-bg-elevated text-fg-subtle">
                  <tr>
                    <th className="px-4 py-3 font-medium">Route</th>
                    <th className="px-4 py-3 font-medium">Behavior</th>
                  </tr>
                </thead>
                <tbody>
                  {routes.map((r, i) => (
                    <tr
                      key={r.name}
                      className={i % 2 === 0 ? "bg-bg" : "bg-bg-elevated/50"}
                    >
                      <td className="px-4 py-3 font-mono text-xs font-medium text-fg">
                        {r.name}
                      </td>
                      <td className="px-4 py-3 text-fg-muted">{r.desc}</td>
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
          </div>
        </section>

        {/* Usage */}
        <section className="border-b border-border">
          <div className="mx-auto max-w-5xl px-4 py-14 sm:px-6 sm:py-16">
            <h2 className="text-sm font-medium uppercase tracking-[0.12em] text-fg-subtle">
              Usage
            </h2>
            <div className="mt-6 overflow-x-auto rounded-[var(--radius-lg)] border border-border bg-bg-elevated p-5">
              <pre className="font-mono text-xs leading-relaxed text-fg sm:text-sm">
                <code>{`#include "GeblomiSort.hpp"

std::vector<int> v = /* ... */;
geblomi::sort(v.begin(), v.end());
geblomi::sort(v.begin(), v.end(), std::greater<>{});
geblomi::sort(v);  // container overload`}</code>
              </pre>
            </div>
            <p className="mt-4 text-sm text-fg-muted">
              Requirements: C++20, random-access iterators. Compile with{" "}
              <code className="rounded bg-bg-subtle px-1.5 py-0.5 font-mono text-xs text-fg">
                -std=c++20 -O3
              </code>
              .
            </p>
          </div>
        </section>

        {/* Package + credits */}
        <section className="border-b border-border">
          <div className="mx-auto max-w-5xl px-4 py-14 sm:px-6 sm:py-16">
            <h2 className="text-sm font-medium uppercase tracking-[0.12em] text-fg-subtle">
              Package contents
            </h2>
            <ul className="mt-6 space-y-2 font-mono text-sm text-fg-muted">
              {[
                "GeblomiSort.hpp — amalgamated header (pdqsort + ska_sort + hybrid)",
                "README.md — features, usage, credits",
                "LICENSE — MIT hybrid + retained third-party notices",
                "examples/demo.cpp — 1M-element correctness & timing demo",
                ".gitignore",
              ].map((line) => (
                <li
                  key={line}
                  className="flex gap-2 rounded-[var(--radius-sm)] border border-border bg-bg-elevated px-3 py-2.5"
                >
                  <BookOpen className="mt-0.5 h-3.5 w-3.5 shrink-0 text-fg-subtle" />
                  <span>{line}</span>
                </li>
              ))}
            </ul>

            <div className="mt-10 rounded-[var(--radius-lg)] border border-border bg-bg-elevated p-5 sm:p-6">
              <h3 className="text-sm font-semibold text-fg">Credits & licenses</h3>
              <ul className="mt-4 space-y-3 text-sm text-fg-muted">
                <li>
                  <span className="font-medium text-fg">pdqsort</span> — Orson Peters
                  (2021), zlib-style (notice retained in header)
                </li>
                <li>
                  <span className="font-medium text-fg">ska_sort</span> — Malte Skarupke
                  (2016), Boost Software License 1.0
                </li>
                <li>
                  <span className="font-medium text-fg">Geblomi hybrid</span> — Grok +
                  Harper + Benjamin + Lucas + Heywood, MIT
                </li>
              </ul>
            </div>
          </div>
        </section>

        {/* CTA */}
        <section>
          <div className="mx-auto max-w-5xl px-4 py-14 sm:px-6 sm:py-20">
            <div className="rounded-[var(--radius-xl)] border border-border-strong bg-bg-elevated p-6 sm:p-10">
              <h2 className="text-xl font-semibold tracking-tight text-fg sm:text-2xl">
                Ready for official publish
              </h2>
              <p className="mt-3 max-w-xl text-sm leading-relaxed text-fg-muted sm:text-base">
                Follow the guided steps to create the public GitHub repo, push this
                package, link your profile to{" "}
                <a
                  href="https://x.com/HeywoodGeblomi"
                  className="text-fg underline decoration-border-strong underline-offset-4 hover:decoration-fg"
                  target="_blank"
                  rel="noopener noreferrer me"
                >
                  @HeywoodGeblomi
                </a>
                , and announce the release.
              </p>
              <div className="mt-6 flex flex-wrap gap-3">
                <Link to="/publish">
                  <Button size="lg">
                    Open publish guide
                    <ArrowRight className="h-4 w-4" />
                  </Button>
                </Link>
                <a href="/geblomi-sort/README.md" download>
                  <Button size="lg" variant="secondary">
                    <Download className="h-4 w-4" />
                    README.md
                  </Button>
                </a>
              </div>
            </div>
          </div>
        </section>
      </main>

      <footer className="border-t border-border">
        <div className="mx-auto flex max-w-5xl flex-col gap-3 px-4 py-8 text-xs text-fg-subtle sm:flex-row sm:items-center sm:justify-between sm:px-6">
          <p>GeblomiSort v2.5 · Buffer-safe adaptive hybrid</p>
          <a
            href="https://x.com/HeywoodGeblomi"
            target="_blank"
            rel="noopener noreferrer me"
            className="inline-flex items-center gap-1.5 text-fg-muted transition-colors hover:text-fg"
          >
            <XIcon className="h-3.5 w-3.5" />
            @HeywoodGeblomi
          </a>
        </div>
      </footer>
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
