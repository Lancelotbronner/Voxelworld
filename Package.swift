// swift-tools-version: 5.8
import PackageDescription

var package = Package(
	name: "Voxelworld",
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
			],
			resources: [
				.copy("textures"),
				.copy("shaders"),
				.copy("timeofday.txt"),
			],
			linkerSettings: [
				.linkedFramework("Cocoa"),
				.linkedLibrary("curl"),
			]
		),

		// Dependencies

		.binaryTarget(
			name: "glfw3",
			path: "Dependencies/glfw/glfw3.xcframework"
		),

		.target(name: "noise", path: "Dependencies/noise"),
		.target(name: "tinycthread", path: "Dependencies/tinycthread"),
		.target(name: "lodepng", path: "Dependencies/lodepng"),
	],
	cLanguageStandard: .c17
)
