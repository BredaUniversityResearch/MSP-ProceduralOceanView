# 🌊 Immersive Ocean (formerly Procedural Ocean View)

**An open-source Unreal Engine 5 platform for immersive, data-driven 3D visualisation of maritime environments.**

![Main Menu](Media/image-10.png)

Immersive Ocean (IO) transforms two-dimensional geospatial data into real-time, interactive 3D maritime environments — complete with wind farms, shipping lanes, marine ecosystems, underwater terrain, and live weather. It is designed to support **Maritime Spatial Planning (MSP)** workshops, education, and research, and runs on both **PC** and **VR** (Meta Quest 2/3).


![Main 3D world](Media/image-11.png)


IO is developed by [Cradle Lab](https://www.buas.nl) at **Breda University of Applied Sciences** and was originally created as part of the EU H2020 [ILIAD project](https://www.ocean-twin.eu/) (Grant Agreement No. 101037643). It works alongside the [MSP Challenge](https://www.mspchallenge.info/) serious game, where co-located teams of stakeholders collaboratively develop and discuss maritime spatial plans.

> **Video:** [Procedural Ocean Trailer](https://www.youtube.com/watch?v=PLACEHOLDER) · [Webinar on Procedural Content Generation in IO](https://www.youtube.com/watch?v=PLACEHOLDER)

---

## Table of Contents

- [Features](#features)
- [How It Works](#how-it-works)
- [Architecture](#architecture)
- [System Requirements](#system-requirements)
- [Getting Started](#getting-started)
  - [1. Install Unreal Engine](#1-install-unreal-engine)
  - [2. Clone the Repository](#2-clone-the-repository)
  - [3. Import Data with POV Launcher](#3-import-data-with-pov-launcher)
  - [4. Run the Application](#4-run-the-application)
- [Using with MSP Challenge](#using-with-msp-challenge)
- [Procedural Rule Packs (PRPs)](#procedural-rule-packs-prps)
  - [Existing Rule Packs](#existing-rule-packs)
  - [Creating Your Own PRP](#creating-your-own-prp)
- [Data Integration](#data-integration)
  - [Supported Formats](#supported-formats)
  - [Weather Integration](#weather-integration)
  - [Geographic Projections](#geographic-projections)
- [Controls](#controls)
  - [PC Controls](#pc-controls)
  - [VR Controls](#vr-controls)
- [Building for Distribution](#building-for-distribution)
- [Known Limitations and Paid Assets](#known-limitations-and-paid-assets)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [Research and Publications](#research-and-publications)
- [Licence](#licence)
- [Acknowledgements](#acknowledgements)

---

## Features

- **Procedural 3D environment generation** — Automatically generates immersive maritime worlds from 2D geospatial data using modular Procedural Rule Packs (PRPs).
- **Dual-mode experience** — Runs in both standard PC (mouse & keyboard) and immersive VR (Meta Quest 2/3) modes.
- **Real-time weather** — Initialises with live weather data from [Open-Meteo](https://open-meteo.com/) (precipitation, cloud cover, wind speed/direction, wave height). Users can adjust conditions manually.
- **MSP Challenge integration** — Directly imports maritime spatial plans exported as JSON from the [MSP Challenge](https://www.mspchallenge.info/), converting planned infrastructure, ecology, and shipping into interactive 3D.
- **Data-driven and extensible** — Add new maritime elements by creating new PRPs without modifying core code. Supports GeoJSON, Shapefile, GeoTIFF, WFS, WMS, WCS, and JSON inputs.
- **Multiple data layers** — Toggle visibility of fish populations, shipping routes, wind farms, energy cabling, noise heatmaps, and more through an in-game UI.
- **Time and weather simulation** — Control time of day, cloud cover, wave height, wind speed, visibility, and water turbidity. A time bar allows simulating environmental changes over time.
- **Performance-optimised** — Uses Vertex Animation Textures for fish, shader-based buoyancy, simplified VR skybox, LOD streaming, and radius-based vessel culling to maintain smooth performance.
- **Open data standards** — Built on EPSG:3035 projection with support for OGC protocols and interoperability with GeoServer and QGIS.


![Main 3D world](Media/image-13.png)

![Aquaculture Configuration](Media/image-16.png)

![Particle Simulation](Media/image-17.png)



---

## How It Works

Immersive Ocean is designed to be used as a complement to the MSP Challenge serious game in workshop settings:

1. **Workshop participants** use MSP Challenge to collaboratively create maritime spatial plans (placing wind farms, defining shipping lanes, establishing marine protected areas, etc.) in competitive sessions that typically last 2+ hours.
2. **Plans are exported** from MSP Challenge as a JSON configuration file.
3. **Procedural Ocean ingests the JSON** and procedurally generates a full 3D maritime environment from it — terrain, infrastructure, marine life, weather, and all.
4. **Participants explore** their plan immersively (on PC or in VR), gaining a visceral understanding of the spatial relationships and impacts of their decisions.

The tool can also be used standalone with any compatible geospatial data — you do not need MSP Challenge to use it.

---

## Architecture

IO is built around a modular, data-driven pipeline:

```
┌─────────────────┐     ┌──────────────┐     ┌─────────────────────┐     ┌───────────┐
│  Data Sources    │────▶│  Data Parser  │────▶│     Data Pool       │────▶│  3D World  │
│                  │     │              │     │  (centralised repo) │     │           │
│ • MSP Challenge  │     │ • JSON       │     │ • Bathymetry        │     │ • Terrain  │
│   (JSON config)  │     │ • GeoTIFF    │     │ • Sediments         │     │ • Assets   │
│ • Open-Meteo API │     │ • Shapefile  │     │ • Wind data         │     │ • Sky      │
│ • GeoServer      │     │ • WFS/WMS    │     │ • Ecology           │     │ • Water    │
│ • EMODnet        │     └──────────────┘     │ • Infrastructure    │     └───────────┘
│ • Copernicus     │                          └─────────┬───────────┘           │
└─────────────────┘                                     │                      │
                                                        ▼                      ▼
                                              ┌─────────────────┐     ┌───────────────┐
                                              │ Procedural Rule │     │ 3D Interface  │
                                              │ Packs (PRPs)    │     │               │
                                              │                 │     │ • UI layers    │
                                              │ • Wind Turbines │     │ • Controls     │
                                              │ • Fish          │     │ • Camera       │
                                              │ • Shipping      │     │ • Mini map     │
                                              │ • Benthos       │     └───────────────┘
                                              │ • Wave Turbines │
                                              │ • Fishing       │
                                              │   Vessels       │
                                              └─────────────────┘
```

The **Data Pool** acts as the backbone — when any data source changes (e.g. a weather update or a shift in ocean currents), changes propagate automatically to all registered PRPs and the 3D world updates in real time.

---

## System Requirements

### PC (Minimum)

| Component | Specification |
|-----------|--------------|
| OS | Windows 10 64-bit (v1909 rev. 1350+, or v2004/20H2 rev. 789+) |
| CPU | Quad-core Intel or AMD, 2.5 GHz+ |
| RAM | 16 GB |
| GPU | NVIDIA GeForce RTX 3060 or AMD RX 6700 |
| Storage | 64 GB SSD |

### PC (Recommended)

| Component | Specification |
|-----------|--------------|
| CPU | Six-core Xeon E5-2643 @ 3.4 GHz (or equivalent) |
| RAM | 64 GB |
| GPU | NVIDIA GeForce RTX 2080 Super or better |
| Storage | 256 GB SSD (+ 2 TB external SSD for data) |

### VR (Additional Requirements)

| Headset | Status |
|---------|--------|
| Meta Quest 2 | Minimum (B&W passthrough, Snapdragon XR2 Gen 1, 6 GB RAM) |
| Meta Quest 3 | Recommended (colour passthrough, Snapdragon XR2 Gen 2, 8 GB RAM) |

VR headsets must be connected to PC via **Meta Link** (or equivalent). The VR experience is PC-tethered, not standalone.

### Software

- **Unreal Engine 5.3.x** — Install via the [Epic Games Launcher](https://store.epicgames.com/en-US/download)
- **Git** — For cloning the repository
- **Meta Link** (or SteamVR) — For VR mode only

---

## Getting Started

### 1. Install Unreal Engine

1. Download and install the [Epic Games Launcher](https://store.epicgames.com/en-US/download).
2. Open the launcher, go to **Unreal Engine** → **Library**.
3. Click the **+** icon next to "Engine Versions" and install **Unreal Engine 5.3.x**.

### 2. Clone the Repository

```bash
git clone https://github.com/BredaUniversityResearch/MSP-ProceduralOceanView.git
```

> ⚠️ **Note:** This open-source distribution omits certain paid Unreal Marketplace assets that cannot be freely redistributed. See [Known Limitations and Paid Assets](#known-limitations-and-paid-assets) for details.

Open the `.uproject` file in the cloned folder with Unreal Engine 5.3.

### 3. Import Data with POV Launcher

The **POV Launcher** (`pov-launcher.exe`) is the entry point for loading maritime data into the application.

**Option A: Import from MSP Challenge**

1. Run `pov-launcher.exe` and choose **"Import POV Config"**.
2. Select your MSP Challenge JSON export file.
3. Enter the region coordinates. You can look up coordinates using [epsg.io/3035](https://epsg.io/3035).
4. Leave remaining fields at their defaults unless you have specific requirements.

**Option B: Use existing scene data**

If you already have imported scenes, you can skip the import step and go directly to launching in Desktop or VR mode.

### 4. Run the Application

From the POV Launcher:

| Option | Description |
|--------|-------------|
| **Import POV Config** | Import a new JSON configuration from MSP Challenge |
| **Start POV in Desktop mode** | Launch the 3D experience with mouse & keyboard (or run `MSP_ProceduralGen.exe` directly) |
| **Launch POV in VR mode** | Launch in VR (ensure your headset is connected via Meta Link first) |

Select your imported scene and explore.


![Underwater](Media/image-12.png)



---

## Using with MSP Challenge

[MSP Challenge](https://www.mspchallenge.info/) is a serious game platform for collaborative maritime spatial planning. In a typical workshop:

1. Participants play different stakeholder roles (energy companies, fisheries, environmental groups, shipping authorities, governments).
2. They collaboratively plan how to use the ocean over the next 10–30 years.
3. Plans are exported as a JSON configuration file.
4. That file is imported into Procedural Ocean, which generates a full 3D visualisation of the plan.

This workflow enables participants to not just plan in 2D, but to actually *experience* their plan — seeing how wind turbines look at scale, how shipping lanes interact with marine habitats, and what the underwater environment looks like beneath their planned infrastructure.

The data flow is: **MSP Challenge → GeoServer → JSON export → IO (via POV Launcher)**.


![Shipping Routes](Media/image-28.png)

---

## Procedural Rule Packs (PRPs)

PRPs are the core of IO's content generation system. Each PRP is a self-contained, modular content package that defines how a specific type of maritime element is generated, visualised, and interacted with.

### Existing Rule Packs

| PRP | Description |
|-----|-------------|
| **North Sea Fish** | Generates fish populations based on ecological data. Uses Vertex Animation Textures for performant animation. Density and species distribution driven by data layers. |
| **Wind Turbines** | Places wind turbines and transformer stations. Turbine rotation speed responds to real-time wind data. Supports construction phase simulation. |
| **Wave Turbines** | Generates wave energy devices with shader-based buoyancy. |
| **Fishing Vessels** | Spawns and animates vessels along shipping routes. Only renders vessels within a 1 km extended radius for performance. |
| **Benthos** | Visualises benthic (seafloor) organisms and habitat distribution. |
| **Shipping** | Renders shipping lanes and vessel traffic patterns from geospatial data. |

### Creating Your Own PRP

Each PRP requires four components:

1. **Manifest** — A Data Asset (`Procedural Rule Pack Manifest`) that tells the system which Processors and Display Modes to execute.
2. **Processor** — A Blueprint extending `Procedural Rule Pack Data Source Processor`. Reads from the Data Pool (typically JSON-derived) and creates data layers.
3. **Display Mode** — A Blueprint extending `Procedural Rule Pack Display Mode`. Reads a data layer and spawns/manages 3D actors.
4. **PRP Actor** — A Blueprint extending `Procedural Rule Pack Actor`. The actual 3D model with its behaviour logic.

**Folder structure** for a new PRP (e.g., Fish Farms):

```
Content/PRPPacks/FishingFarms/
├── 00_Assets/
│   ├── 00_Blueprints/
│   ├── 01_Meshes/
│   ├── 02_Materials/
│   └── 03_Textures/
├── 01_DisplayModes/
├── 02_Processors/
└── 03_Manifest/
```

For a detailed step-by-step tutorial, see the **Procedural Rule Pack Guide** PDF in the `Documentation/` folder.

---

## Data Integration

### Supported Formats

| Format | Type | Notes |
|--------|------|-------|
| **JSON** | Primary | Configuration files from MSP Challenge |
| **GeoJSON** (RFC 7946) | Vector | Standard geospatial format |
| **Shapefile** (.shp) | Vector | Widely used in GIS |
| **GeoTIFF** | Raster | Bathymetry, terrain data |
| **WFS** | Service | Web Feature Service (OGC) |
| **WMS** | Service | Web Map Service (OGC) |
| **WCS** | Service | Web Coverage Service (OGC) |

Additional formats (netCDF, CSV, database connections) can be processed through [QGIS](https://qgis.org/) and converted into supported formats.

### Weather Integration

IO connects to the [Open-Meteo](https://open-meteo.com/) free weather API at startup to initialise the scene with real-world weather conditions for the selected location. Live parameters include:

- Precipitation
- Cloud cover
- Wind speed and direction
- Wave height

Users can modify all weather parameters through the in-game UI and reset to live data at any time.

![Live weather](Media/image-29.png)

### Geographic Projections

The primary projection is **ETRS89-extended / LAEA Europe (EPSG:3035)**, suitable for European maritime regions. Currently tested regions include the North Sea, Baltic Sea, Adriatic Sea, and Celtic Sea / Firth. New regions can be added by providing appropriate projection parameters and data layers.

### Data Sources

IO's data pipeline ultimately draws from European marine data infrastructure:

- [**EMODnet**](https://emodnet.ec.europa.eu/) — European Marine Observation and Data Network
- [**Copernicus**](https://www.copernicus.eu/) — EU Earth observation programme
- [**EDITO**](https://edito.eu/) — European Digital Twin of the Ocean (compatible)
- [**GeoServer**](http://geoserver.org/) — Open-source server for sharing geospatial data (used by MSP Challenge)

Data typically flows through MSP Challenge, which retrieves geospatial data from GeoServer and exports it as JSON for IO. IO can also ingest data directly if provided in a supported format.

--- 
# MSP-ProceduralOceanView

## Directly export data from MSP Challenge into Procedural Ocean using POV launcher

1. Start pov-launcher.exe application in the ProceduralOcean and choose "Import POV Config"

![alt text](Media/image-7.png)


select option 2, and use the coordinates you entered. 

![alt text](Media/image-8.png)

You may leave the rest blank/default.

2. Play and have fun, either in Desktop or VR mode.

![alt text](Media/image-9.png)

---

## Controls

### PC Controls

| Action | Control |
|--------|---------|
| Move | WASD |
| Look around | Hold right mouse button + move mouse |
| Interact with objects | Left click |
| Toggle UI elements | In-game menu icons |
| Hide interface (screenshots) | Auto-hide feature |
| Adjust weather | UI sliders (time of day, cloud cover, wave height, wind, visibility, turbidity) |
| Time control | Horizontal bar at top of screen (0.5×, 1×, 2×, 10× speed) |

### VR Controls

| Action | Control |
|--------|---------|
| Move | Point controller in direction + grip button |
| Snap turn (45°) | Swipe trackpad / joystick |
| Interact with objects | Point controller + trigger |
| Open main menu | Rotate hand sideways (like checking a watch) |
| Navigate menu | Point with other controller + trigger |


![Navigation](Media/image-27.png)

---

## Building for Distribution

To create a standalone executable build:

1. Follow the [Unreal Engine Build Guide](https://dev.epicgames.com/documentation/en-us/unreal-engine/preparing-unreal-engine-projects-for-release?application_version=5.3).
2. Note that builds from the open-source repository may be missing visual components that depend on paid Marketplace assets. See the section below.

---

## Known Limitations and Paid Assets

### Paid Asset Dependencies

This open-source repository **omits certain paid Unreal Marketplace assets** that were used during development but cannot be freely redistributed. These include:

- **Water shader** (Waterline PRO) — The water rendering system is a paid Marketplace asset. The open-source version will have degraded or missing water visuals without it. Unreal Engine's built-in water system is not currently recommended (it has been in beta for years).
- **Certain 3D model assets** — Some visual assets (meshes, materials, textures) sourced from the Marketplace during earlier development are excluded.
- **NVIDIA DLSS components** — DLSS, NIS, and StreamLine plugins are freely available from [NVIDIA](https://developer.nvidia.com/rtx/dlss/get-started#ue-version) and must be obtained separately.

**What this means in practice:** If you clone this repository, you will get a fully functional procedural generation system, but some visual assets may appear as placeholder materials or be absent. The Procedural Rule Packs system itself is fully open source, and you can substitute your own 3D assets.

### Other Limitations

- **VR was retrofitted** onto a PC-first design, resulting in a less-than-ideal VR experience in some areas (menu interactions, navigation).
- The platform currently targets **European maritime regions** (EPSG:3035 projection). Other regions require additional projection configuration.
- There is no onboarding tutorial built into the application.

---

## Project Structure

```
MSP-ProceduralOceanView/
├── Config/                     # Unreal project configuration
├── Content/
│   ├── PRPPacks/               # Procedural Rule Packs
│   │   ├── NorthSeaFish/
│   │   ├── WindTurbines/
│   │   ├── WaveTurbines/
│   │   ├── FishingVessels/
│   │   ├── Benthos/
│   │   └── Shipping/
│   ├── Maps/                   # Level files
│   ├── UI/                     # User interface assets
│   └── ...
├── Documentation/
│   ├── ProceduralRulePackGuide.pdf
│   └── ...
├── Source/                     # C++ source code
├── pov-launcher.exe            # Data import and launch utility
├── MSP_ProceduralGen.uproject  # Unreal project file
└── README.md
```

> ⚠️ This structure is indicative. Refer to the actual repository contents for the definitive layout.

---

## Contributing

We welcome contributions from the community. To contribute:

1. **Fork** the repository.
2. **Create a branch** for your feature or fix.
3. **Follow the PRP pattern** when adding new maritime content — see the [Procedural Rule Pack Guide](Documentation/ProceduralRulePackGuide.pdf).
4. **Submit a pull request** with a clear description of your changes.

All contributions must be compatible with the **GPL-3.0** licence. Do not include paid Marketplace assets or any assets you do not have the right to redistribute.

### Areas Where Contributions Are Especially Welcome

- New Procedural Rule Packs (aquaculture, oil platforms, marine protected areas, coral reefs, etc.)
- Improved VR interaction and navigation
- Support for additional geographic projections and regions
- Documentation and tutorials
- Performance optimisations
- Accessibility improvements

---


## Licence

The Procedural Ocean Platform and its source code are provided under the **[GNU General Public License Version 3 (GPL-3.0-only)](https://www.gnu.org/licenses/gpl-3.0.en.html)**.

You may freely copy, distribute, and modify the software provided that:

- You track changes and their dates in source files.
- Any modifications are also made available under GPL-3.0, along with build and install instructions.

**Third-party components** have their own licences:

- NVIDIA DLSS/NIS/StreamLine — [NVIDIA licence](https://developer.nvidia.com/rtx/dlss/get-started#ue-version)
- GIS data sources retain their original copyright (we do not redistribute originals)
- Paid Unreal Marketplace assets are **not included** and are **not covered** by this licence

Full credits of developers, partners, and funders are available on the [MSP Challenge Community Wiki](https://community.mspchallenge.info/).

---

## Acknowledgements

Procedural Ocean was developed by [Cradle Lab](https://www.buas.nl) at **Breda University of Applied Sciences**, the Netherlands.

**Core development team:** Cradle research and development team.

**Funded by:**

- European Union's Horizon 2020 research and innovation programme under Grant Agreement No. **101037643** (ILIAD project)

**Built with:**

- [Unreal Engine 5](https://www.unrealengine.com/) by Epic Games
- [MSP Challenge](https://www.mspchallenge.info/)
- [Open-Meteo](https://open-meteo.com/) weather API
- [GeoServer](http://geoserver.org/)
- [QGIS](https://qgis.org/)

---

<p align="center">
  <em>Breda University of Applied Sciences · Cradle Lab · 2022–2025</em>
</p>
