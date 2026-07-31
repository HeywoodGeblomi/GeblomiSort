import { M as require_jsx_runtime, c as HeadContent, d as Outlet, f as lazyRouteComponent, m as createRootRoute, p as createFileRoute, s as Scripts, u as createRouter } from "../_libs/@tanstack/react-router+[...].mjs";
//#region node_modules/.nitro/vite/services/ssr/assets/router-TXijcG51.js
var import_jsx_runtime = require_jsx_runtime();
var styles_default = "/assets/styles-BAKJFTSd.css";
var Route$2 = createRootRoute({
	head: () => ({
		meta: [
			{ charSet: "utf-8" },
			{
				name: "viewport",
				content: "width=device-width, initial-scale=1"
			},
			{ title: "GeblomiSort — Adaptive Hybrid C++20 Sorting" },
			{
				name: "description",
				content: "Commercial-grade adaptive hybrid 1-D sorting algorithm. Header-only C++20. Official project by @HeywoodGeblomi."
			},
			{
				name: "theme-color",
				content: "#0a0a0b"
			}
		],
		links: [{
			rel: "stylesheet",
			href: styles_default
		}, {
			rel: "me",
			href: "https://x.com/HeywoodGeblomi"
		}]
	}),
	component: RootDocument
});
function RootDocument() {
	return /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("html", {
		lang: "en",
		suppressHydrationWarning: true,
		children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)("head", { children: /* @__PURE__ */ (0, import_jsx_runtime.jsx)(HeadContent, {}) }), /* @__PURE__ */ (0, import_jsx_runtime.jsxs)("body", {
			className: "min-h-dvh bg-bg text-fg antialiased",
			children: [/* @__PURE__ */ (0, import_jsx_runtime.jsx)(Outlet, {}), /* @__PURE__ */ (0, import_jsx_runtime.jsx)(Scripts, {})]
		})]
	});
}
var $$splitComponentImporter$1 = () => import("./routes-CqO2dgmC.mjs");
var Route$1 = createFileRoute("/")({ component: lazyRouteComponent($$splitComponentImporter$1, "component") });
var $$splitComponentImporter = () => import("./publish-C7k5H4P9.mjs");
var Route = createFileRoute("/publish")({ component: lazyRouteComponent($$splitComponentImporter, "component") });
var rootRouteChildren = {
	IndexRoute: Route$1.update({
		id: "/",
		path: "/",
		getParentRoute: () => Route$2
	}),
	PublishRoute: Route.update({
		id: "/publish",
		path: "/publish",
		getParentRoute: () => Route$2
	})
};
var routeTree = Route$2._addFileChildren(rootRouteChildren)._addFileTypes();
function getRouter() {
	return createRouter({
		routeTree,
		defaultPreload: "intent",
		scrollRestoration: true
	});
}
//#endregion
export { getRouter };
