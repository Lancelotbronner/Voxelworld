// swift-tools-version: 5.8
import PackageDescription

var package = Package(
	name: "Voxelworld",
	platforms: [
		.macOS(.v10_15),
	],
	dependencies: [
		.package(url: "https://github.com/recp/cglm", branch: "master"),
	],
	targets: [

//		.executableTarget(
//			name: "Voxelworld"
//		),

		.executableTarget(
			name: "CVoxelworld",
			dependencies: [
				"glfw3",
				"noise",
				"tinycthread",
				"lodepng",
				.product(name: "cglm", package: "cglm"),
			],
			resources: [
				.copy("textures"),
				.copy("shaders"),
				.copy("timeofday.txt"),
			],
			cSettings: [
				.define("GLFW_INCLUDE_GLCOREARB"),
				.define("GL_SILENCE_DEPRECATION"),
			],
			linkerSettings: [
				.linkedFramework("Cocoa"),
				.linkedLibrary("curl"),
			]
		),

		// Dependencies

		.binaryTarget(name: "glfw3", path: "Dependencies/glfw/glfw3.xcframework"),

		.target(name: "noise", path: "Dependencies/noise"),
		.target(name: "tinycthread", path: "Dependencies/tinycthread"),
		.target(name: "lodepng", path: "Dependencies/lodepng"),
	],
	cLanguageStandard: .c2x
)
