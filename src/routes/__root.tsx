import { createRootRoute, HeadContent, Outlet, Scripts } from "@tanstack/react-router";
import appCss from "../styles.css?url";

export const Route = createRootRoute({
  head: () => ({
    meta: [
      { charSet: "utf-8" },
      { name: "viewport", content: "width=device-width, initial-scale=1" },
      {
        title: "GeblomiSort — Adaptive Hybrid C++20 Sorting",
      },
      {
        name: "description",
        content:
          "Commercial-grade adaptive hybrid 1-D sorting algorithm. Header-only C++20. Official project by @HeywoodGeblomi.",
      },
      { name: "theme-color", content: "#0a0a0b" },
    ],
    links: [
      { rel: "stylesheet", href: appCss },
      { rel: "me", href: "https://x.com/HeywoodGeblomi" },
    ],
  }),
  component: RootDocument,
});

function RootDocument() {
  return (
    <html lang="en" suppressHydrationWarning>
      <head>
        <HeadContent />
      </head>
      <body className="min-h-dvh bg-bg text-fg antialiased">
        <Outlet />
        <Scripts />
      </body>
    </html>
  );
}
