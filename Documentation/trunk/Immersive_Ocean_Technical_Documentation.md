# Immersive Ocean Platform - Technical Documentation

## A Virtual Twin for Maritime Spatial Planning

---

**Authors:** Xavier Fonseca, Jens Hagen, Igor Mayer, Carlos Pereira Santos, Phil De Groot, Kevin Hutchinson, Joey Relouw

**Affiliation:** Academy for AI, Games and Media, Breda University of Applied Sciences, Breda, Netherlands

---

## Abstract

Digital Twins of the Ocean (DTOs) are increasingly used in Maritime Spatial Planning (MSP), yet most remain limited to 2D representations and offer minimal stakeholder interactivity. These limitations reduce their effectiveness in capturing complex socio-ecological-technical dynamics and supporting exploratory what-if scenario planning in a 3D or 4D ocean space. This document presents Immersive Ocean, a novel Virtual Twin platform developed within EU-ILIAD DTO initiative. Built with game engine and VR technologies, it supports procedural 3D world generation and interactive exploration in both desktop and immersive VR modes. Systematic performance validation demonstrated stable frame rates across both PC and VR platforms. Initial user evaluations (n=22) report high usability and engagement but also suggest areas for improvement in UI clarity and ecological model representation. These initial findings position Immersive Ocean as a promising Virtual Twin solution for an immersive, interactive, and data-integrated approach to MSP and ocean governance. Immersive Ocean is now being piloted with stakeholders in real-world MSP scenarios, including offshore wind farm planning.

**Keywords:** Immersive Ocean, Maritime Spatial Planning, Virtual Twin for the Ocean, Digital Twin, Procedural Generation, Virtual Reality

---

## 1. Introduction

The European Union's ILIAD project[1] is part of a broader European effort to develop a Digital Twin of the Ocean (DTO), alongside sister initiatives such as EDITO[2]. DTOs are dynamic, digital representations of real-world maritime environments characterized by their integration of high-resolution geospatial data, real-time sensor data, and computational simulation models [3-5]. These DTO can be used for many different purposes, including but not limited to the integration of scientific knowledge (data, models), planning and decision-support (what if scenarios), stakeholder involvement (communication, social learning), which are all relevant in a context of Maritime Spatial Planning (MSP) and Ocean Governance [6-8].

Despite notable advancements, existing DTO implementations often rely on predefined data models and constrained simulation frameworks, which limit interactivity and flexibility in scenarios [9-18]. While dynamic in data integration, most tools primarily support observational analysis rather than active exploration or intervention by multiple users [19-22]. This presents substantial limitations for MSP, including reduced stakeholder agency, limited what-if scenario planning, and constrained engagement in collaborative processes [23, 24]. Furthermore, current DTO tools often struggle to represent the entire spatial and temporal complexity of marine systems, where key interactions unfold not only horizontally but also vertically through the water column [5, 25]. These include dynamic oceanographic processes, vertical species migration, and the three-dimensional interplay between human activities in various sectors—such as the spatial and temporal interactions between offshore wind farm construction, shipping, fishing pressure, and seabird behaviour [10, 24, 26-28]. As a result, users are often unable to interactively test or adapt spatial strategies within these multi-dimensional and time-bound environments, limiting the effectiveness of DTOs as decision-support tools [29].

Acknowledging these limitations, the EU-funded ILIAD DTO project[1] introduces Immersive Ocean—a Virtual Twin platform that reimagines the traditional DTO framework in a highly immersive, interactive, and 3D representation. Complementary to conventional DTOs, Immersive Ocean integrates Procedural Generation—a form of generative AI commonly used in game and immersive technologies—with dynamic 3D simulations and immersive Virtual Reality (VR). This combination enables a scenario-driven, interactive experience in which stakeholders can actively explore and influence complex marine environments. By generating realistic 3D marine scenarios from 2D geospatial data and real-time environmental inputs, Immersive Ocean directly addresses key constraints of current DTOs, particularly in terms of interactivity, spatial realism, and stakeholder engagement.

This document presents the Immersive Ocean platform (in its present edition), highlighting its core innovations, functionalities, and methodological framework. Furthermore, it provides a preliminary assessment of its technical performance, usability, and applicability to MSP. Through this evaluation, the document contributes valuable insights into the potential of immersive, procedurally generated virtual twins to transform stakeholder engagement, data integration, and decision-making processes in maritime contexts.

---

## 2. Immersive Ocean Virtual Twin

Despite the increasing adoption of DTO in MSP [6] significant technological and usability challenges persist, limiting their effectiveness, broader adoption, and stakeholder engagement. A primary limitation is the absence of fully interactive and immersive environments, as most MSP tools continue relying on static GIS-based 2D or 3D visualizations rather than fully immersive Virtual or Augmented Reality interfaces [30, 31]. These conventional platforms fail to substantially enhance realism or enable stakeholders to intuitively and immersively engage with virtual maritime environments [19]. Additionally, stakeholder engagement remains constrained, as many digital twin platforms require specialized GIS or programming expertise, reducing accessibility for local communities, small-scale maritime industries, and non-expert policymakers [30].

The Immersive Ocean platform[32] addresses these limitations by integrating procedural 3D generation, dynamic real-time simulations, and an intuitive VR-enabled interface. These innovations significantly enhance spatial awareness, collaborative planning, and scenario-driven decision support within MSP contexts. Immersive Ocean dynamically generates realistic 3D marine environments from 2D geospatial data and real-time environmental inputs, integrating live data streams such as current weather conditions into user-defined planning scenarios. This enables users to explore, manipulate, and interact intuitively with complex maritime scenarios, fostering more informed, participatory, and immersive decision-making processes and effectively bridge the gap between analytical decision-support systems and experiential planning tools.

### 2.1 Core Functionalities

The Immersive Ocean incorporates several key capabilities that enhance its usability, realism, and adaptability in MSP. These are covered as:

1. User interface and interaction
2. Data models
3. Procedural generation
4. Data integration
5. Interoperability and extensibility

#### 2.1.1 User Interface and Interaction

Immersive Ocean employs an interactive user interface built on Unreal Engine 5[33] compatible with both PC and VR modes. The main menu facilitates user interaction through scene selection, utilizing JSON data files to trigger the simulation of specific maritime scenarios based on real-time geospatial data. A sidebar enables the control of various visualization layers, such as energy infrastructure (wind turbines, cables, transformer stations), ecological data (species distribution), and shipping activities, each dynamically represented through detailed 3D models.

Interaction within the PC mode involves object selection for detailed inspection, infrastructure placement via mouse inputs, and simulation control, such as initiating maintenance vessels. Movement employs keyboard navigation (WASD for horizontal and Spacebar/Ctrl for vertical movement), with viewpoint adjustments using mouse controls. The VR interface emphasizes immersion, employing controllers for environment navigation, object manipulation, and accessing menus through intuitive gestures, including hand rotation for menu activation and "snap turn" rotation.

Real-time adjustments of environmental parameters, such as weather conditions (cloud coverage, wind speed, visibility), are accessible through the user interface, enabling users to explore diverse planning scenarios dynamically. The procedural generation system adapts infrastructure behaviour in response to environmental inputs—for example, adjusting turbine rotation speed according to wind intensity. Through integration with standardized data formats (GeoJSON, Shapefile, GeoTIFF), Immersive Ocean ensures compatibility with external datasets. Additionally, the system facilitates integration with other MSP tools by supporting command-line instructions that enable launching specific scenarios in designated geographic areas, effectively complementing existing MSP frameworks.

#### 2.1.2 Interaction with Data Models

Immersive Ocean facilitates interaction with complex maritime data models through flexible visualization capabilities. The system interprets multiple environmental and operational data layers, driven by user-defined JSON configuration files specifying the geographical region and thematic scope of simulations. These files dictate available data layers and the associated visualization options, such as ecological distributions, maritime infrastructure. Detailed 3D visualizations represent marine infrastructure, including shipping vessels, wind turbines, transformer stations, and associated underwater cabling, dynamically instantiated from these configuration files.

Ecological data visualization includes detailed 3D representations of marine species, supporting scenario analysis through species-specific heatmaps. Infrastructure and operational data, such as energy facilities and shipping lanes, are similarly visualized via high-resolution 3D models and spatial overlays, facilitating intuitive exploration and decision-making.

#### 2.1.3 Procedural Generation

Immersive Ocean employs procedural generation to algorithmically produce dynamic 3D maritime environments based on predefined procedural rules and real-world data inputs. Central to this system are **Procedural Rule Packs (PRPs)**, modular components defining the instantiation, spatial placement, and interactive behaviors of maritime entities such as wind farms, fish farms, shipping lanes, and ecological elements. PRPs ensure that generated environments accurately reflect dynamic real-world maritime conditions.

The modular structure of PRPs supports extensibility, enabling integration of new content by creating or modifying rules without affecting existing system components. Each PRP encapsulates specific procedural logic—for instance, the Wind Turbine PRP adjusts turbine rotation speeds dynamically according to real-time wind data, while the Fish Farm PRP alters fish population density and behaviors based on environmental variables or user-defined scenarios.

**Currently implemented PRPs include:**
- North Sea Fish Pack
- Wave Turbines Pack
- Wind Turbines Pack
- Fishing Vessel Pack
- Benthos Pack
- Shipping Pack

#### 2.1.4 Data Integration

Immersive Ocean integrates real-world maritime datasets, including bathymetry, ecological data, and maritime infrastructure, primarily through JSON files. These configuration files define the geographic region, available data layers, and the corresponding user interface elements activated during simulation. The system dynamically adjusts its interface to reflect data availability, enabling users to visualize and interact selectively with specific data layers such as fish populations, shipping lanes, or energy infrastructure. Missing data layers result in associated interface elements being automatically hidden, ensuring a clutter-free and contextually relevant user experience.

Integration with external MSP tools is streamlined through standardized JSON files, which other MSP platforms, such as the MSP Challenge[34] digital twin, can generate. Immersive Ocean accepts command-line parameters allowing direct scenario initiation in specified geographical locations, facilitating seamless interoperability with other MSP systems.

Live environmental conditions, such as weather, are also integrated, with users able to modify parameters like cloud coverage, wind speed, and visibility. Users can revert changes to current conditions retrieved from live data feeds.

---

## 3. System Architecture

### 3.1 Structural Diagram

The structural diagram of Immersive Ocean illustrates its modular and scalable approach supporting dynamic, real-time 3D environment generation for MSP. The system consists of distinct components, each with specific responsibilities:

**Data Parser:** Decodes JSON input data (e.g., wind farms, shipping lanes, fish populations, weather conditions) and integrates real-time weather information via the Open-Meteo API[35], applying fallback values for any missing data. It acts as a centralized repository, synchronizing processed data such as bathymetry, ecological information, and maritime infrastructure across system components.

**Environmental Base:** Generates foundational environmental layers, including terrain, water, and atmospheric conditions, ensuring accurate spatial placement of maritime elements.

**Procedural Rules:** Dynamically instantiate maritime entities (wind turbines, vessels, ecological elements), responding to real-time environmental updates from the Data Pool.

**3D World and User Controls:** Manages the visualization of 3D assets and user interactions, including object selection, environmental parameter adjustments, user interface elements, and navigation controls in both PC and VR modes.

### 3.2 Backend Infrastructure

Immersive Ocean's backend employs a relational SQL MariaDB database compatible with MSP Challenge's internal data model, enabling efficient integration and querying of datasets from multiple global standards. The system also integrates with GeoServer, supporting interoperability through standard Open Geospatial Consortium (OGC) protocols[36].

Maritime scenarios exported from external MSP platforms, such as MSP Challenge, are imported as JSON configuration files, facilitating direct scenario initiation and ensuring seamless interoperability across MSP frameworks.

### 3.3 Interoperability and Integration with External Frameworks

Immersive Ocean ensures robust interoperability and seamless integration with external frameworks by adopting standardized geographic projections, widely accepted input formats, and flexible data integration methodologies.

#### Geographic Projections

Geographic data integration primarily utilizes the EPSG standard, specifically the ETRS89-extended/LAEA Europe projection (EPSG:3035)[37], which is optimized for precise spatial representation of European maritime regions. The system currently integrates geospatial data from the MSP Challenge, encompassing regions such as the North Sea, Baltic Sea, Adriatic Sea, and Celtic Bay. Additionally, its flexible architecture supports seamless extension to new regions by incorporating additional projections and data layers not currently integrated within the MSP Challenge, with minimal adjustments to core components.

The tool can convert any widely used projection into the required format, either programmatically or via GeoServer (the internal data model used by the MSP Challenge), assuming the appropriate projection parameters are available.

#### Input Formats and Data Standards

Immersive Ocean natively processes standardized geospatial formats, including:
- GeoJSON (RFC 7946[38])
- Shapefile (SHP)
- GeoTIFF
- Web Feature Service (WFS)
- Web Map Service (WMS)
- Web Coverage Service (WCS)
- JSON

Integration with widely used external GIS platforms such as QGIS[39] and ArcGIS[40] further enhances compatibility, facilitating the ingestion of additional data formats (e.g., netCDF, CSV, database connections) without requiring significant system alterations.

Data visualized by Immersive Ocean originates predominantly from authoritative European sources like EMODnet[41] and Copernicus[42], typically via the MSP Challenge simulation platform. Nevertheless, its flexible architecture supports integration with alternative external repositories such as EDITO[43], either directly or indirectly.

#### Integration Methodologies

Immersive Ocean operates as a standalone interactive visualization and simulation environment, explicitly designed for seamless integration with external geospatial data sources and existing MSP digital twins and DTO frameworks such as Copernicus, EMODnet, MSP Challenge, and EDITO Data Lake pathways. Consequently, Immersive Ocean is capable of consuming diverse maritime data, including bathymetric, oceanographic, ecological, and environmental datasets, all of which are available in accessible, standardized formats.

Aligned with the Ocean Information Model (OIM) principles, Immersive Ocean supports commonly used standardized data formats and methodologies within the broader EDITO ecosystem. While it is not a data management system, Immersive Ocean can consume and visualize OIM-tagged data sources such as MSP Challenge, EDITO, and other DTO-related tools, when these expose relevant oceanographic data via OIM-compliant APIs.

This emphasis on interoperability ensures that the Immersive Ocean remains strategically aligned with broader DTO initiatives, reinforcing its utility as a decision-support tool within the Iliad and EDITO ecosystems. Unlike platforms focusing solely on data aggregation and processing, Immersive Ocean prioritizes interactive, immersive, and user-centered visualization, significantly enhancing stakeholder engagement and informed decision-making in maritime spatial planning contexts.

---

## 4. System Requirements

### 4.1 Minimum Hardware Requirements

Built for Windows, the minimum hardware requirements for Immersive Ocean are:

- **OS:** Windows 10 64-bit version 1909 revision .1350 or higher, or versions 2004 and 20H2 revision .789 or higher
- **Processor:** Quad-core Intel or AMD, 2.5 GHz or faster
- **Memory:** 16GB RAM
- **Storage:** 64GB SSD
- **Graphics Card:** Nvidia GeForce 3060 or AMD 6700

### 4.2 Recommended Hardware Requirements

For optimal performance, the recommended hardware requirements are:

- **OS:** Windows 10 64-bit version 1909 revision .1350 or higher, or versions 2004 and 20H2 revision .789 or higher
- **Processor:** Six-core Xeon E5-2643 @ 3.4 GHz
- **Memory:** 64GB RAM
- **Internal Storage:** 256GB SSD
- **External Storage:** 2TB SSD
- **Graphics Card:** Nvidia GeForce RTX 2080 Super

---

## 5. Performance Testing

To demonstrate that the Immersive Ocean virtual twin operates stably under demanding conditions, its software is thoroughly assessed using multiple tools and methods. Both the PC and VR versions of Immersive Ocean were tested using a combination of internal (engine-level) and external (system-level) performance profiling tools. Internal testing leveraged Unreal Engine's built-in profilers to analyze frame rates, memory use, and GPU load during specific user interactions. External benchmarking tools assessed system-wide resource usage on the compiled software, simulating real-world deployment.

The evaluation of Immersive Ocean was performed on the following specifications:
- **CPU:** Intel Core Ultra 9, BUS speed of 99.76 MHz, 3 Level Cache, 16 Cores at 2300 MHz
- **Graphics Card:** NVIDIA GeForce RTX 4080 Laptop GPU, GDDR6 12288 MB Memory RAM, bandwidth of 432 GB/s, 1530 MHz Clock
- **Operating System:** Microsoft Windows 11
- **PC Model:** Alienware x16 R2
- **Memory:** 32GB RAM physical memory
- **Display:** 1920 x 1080 pixels

### 5.1 Testing Protocol

Performance testing was conducted on both PC and VR versions of Immersive Ocean using a standardized, repeatable protocol executed ten times per platform. The test scenario, titled "NL P.A. Wind Park + Fish Farm", was selected for its complexity, incorporating the full range of 3D models and interactive features.

**Internal profiling** was performed using Unreal Engine's development environment. Activated tools included:
- Real-time console commands (e.g., Stat Memory, Stat FPS, Stat Unit, ProfileGPU)
- Unreal Insights with tracing flags (e.g., CPU, GPU, Frame, LoadTime)
- GPU Visualizer
- Derived Data Cache statistics

**External benchmarking** tools included CPU-Z, GPU-Z, and FRAPS, capturing CPU/GPU load, temperature, memory usage, and framerate metrics during runtime.

Each iteration included baseline resource measurements, followed by a structured interaction sequence within the scenario, 10 times for both PC and VR versions. Tasks included environmental changes (e.g., toggling rain and wind), infrastructure visualization (e.g., turbine animations, fish tanks, ship routes), interaction with UI features (e.g., activating sound-based heatmaps), and camera movements (e.g., underwater inspection and 360-degree ship flybys). PC-specific interactions also included simulating maintenance and construction vessel deployment via double-click gestures.

### 5.2 Performance Results

The following tables summarize the aggregate metrics collected during performance testing:

**Table 1: Aggregate Metrics for Baseline Condition**

| Metrics | PC (Mean ± Std Dev) | VR (Mean ± Std Dev) |
|---------|---------------------|---------------------|
| GPU Load (%) | 45 ± 7 | 53 ± 14 |
| GPU Memory Clock (MHz) | 2,275 ± 80 | 2,250 ± 0 |
| GPU Clock (MHz) | 2,361 ± 97 | 2,402 ± 5 |
| Memory RAM Used (MB) | 15,523 ± 113 | 21,098 ± 724 |
| GPU Temperature (°C) | 76 ± 1 | 79 ± 3 |
| CPU Temperature (°C) | 100 ± 2 | 95 ± 4 |
| GPU Memory Temperature (°C) | 92 ± 1 | 97 ± 3 |

**Table 2: Aggregate Metrics During Testing Protocol**

| Metrics | PC (Mean ± Std Dev) | VR (Mean ± Std Dev) |
|---------|---------------------|---------------------|
| Average FPS (Internal) | 61 ± 7 | 42 ± 11 |
| Minimum FPS (Internal) | 40 ± 29 | 15 ± 18 |
| Maximum FPS (Internal) | 69 ± 19 | 85 ± 67 |
| Average FPS (External) | 63 ± 6 | 43 ± 7 |
| Minimum FPS (External) | 40 ± 29 | 11 ± 13 |
| Maximum FPS (External) | 69 ± 19 | 88 ± 63 |
| GPU Load (%) | 97 ± 1 | 70 ± 7 |
| GPU Memory Clock (MHz) | 2,255 ± 10 | 2,300 ± 187 |
| GPU Clock (MHz) | 2,261 ± 14 | 2,250 ± 0 |
| Memory RAM Used (MB) | 17,984 ± 1,069 | 21,234 ± 449 |
| GPU Temperature (°C) | 86 ± 1 | 84 ± 2 |
| CPU Temperature (°C) | 96 ± 1 | 100 ± 2 |
| GPU Memory Temperature (°C) | 102 ± 1 | 103 ± 2 |

### 5.3 Performance Analysis

The performance evaluation of Immersive Ocean demonstrates robust runtime stability and rendering efficiency across PC and VR platforms. The performance data collected indicate consistently high frame rates (~61 FPS on PC, ~42 FPS on VR) with low variability. GPU utilization was consistently high (~97% PC, ~70% VR), indicating effective hardware optimization tailored to each platform. Analysis showed a linear relationship between resource consumption (GPU and memory usage) and achieved frame rates, highlighting the absence of performance bottlenecks. Additionally, procedural generation tests confirmed that the system effectively converts user-defined 2D geospatial MSP data into complex, interactive 3D environments without significant computational constraints.

The performance evaluation was limited by a relatively small sample size (10 iterations per platform), which may have restricted its broader generalizability. Additionally, manual data collection methods introduce potential for human error and may miss transient performance variations. Despite these limitations, the overall findings indicate the technical readiness and scalability of the Immersive Ocean as an immersive, interactive decision-support tool for MSP.

---

## 6. User Experience Testing

To assess the user experience of the Immersive Ocean, user testing sessions were conducted to assess specific metrics. These metrics (usability, intuitiveness, and realism) aim to point out the software's effectiveness as an interactive decision-support virtual twin for MSP. User testing involved structured gameplay sessions, talk-aloud protocols, and focus group discussions with young professionals familiar with immersive technologies.

### 6.1 Research Design

#### Protocol
Participants engaged in a structured evaluation protocol that included informed consent, pre-questionnaires, a guided exploration using a Think-Aloud protocol, post-questionnaires, and group discussions. This process ensured detailed, systematic data collection, capturing real-time user interactions and subjective feedback.

#### Participants
The study involved 22 young professionals aged 18-30, selected for their proficiency with immersive technologies but without prior expertise in MSP, thereby minimizing bias from technological unfamiliarity.

#### Location
Sessions took place in controlled environments at Breda University of Applied Sciences, ensuring consistency across tests.

#### Method and Data Collection
Data collection employed a combination of qualitative and quantitative methods, including:
- Audio recordings of real-time user interactions (Think-Aloud)
- Demographic and proficiency pre-questionnaires
- Usability-focused post-questionnaires using a Likert scale
- Semi-structured focus group discussions to gain deeper insights into user experiences and perceptions

#### Data Analysis
Quantitative questionnaire data underwent descriptive statistical analysis. Qualitative audio recordings were transcribed and systematically analyzed using Atlas.ti[44] software, which enables the employment of predefined thematic codes that reflect usability, feedback mechanisms, immersion, procedural generation, and realism. Subsequent manual content analysis ensured a nuanced interpretation of user feedback and the identification of recurrent themes.

### 6.2 User Testing Results

#### Post-Questionnaire Results

The post-gameplay questionnaire results are reported for both PC and VR versions, with responses across five categories: Strongly Disagree (-2), Disagree (-1), Neutral (0), Agree (1), and Strongly Agree (2).

**Ease of Use and Intuitiveness:**
Participants positively rated the ease of use and navigation for both the PC and VR versions. The PC version was well-received (9 positive responses), although three neutral responses suggest room for improvement. Similarly, the VR version was also positively evaluated (9 positive responses), but navigation exhibited a slight learning curve.

**Accuracy and Helpfulness of Feedback:**
Feedback mechanisms received mixed reviews on both platforms. In the PC version, five users found the feedback helpful, while three were neutral and 2 were dissatisfied. The VR version showed a broader distribution, with 6 positive responses, 5 neutral, and 1 negative, indicating that clarity of provided information could be improved.

**Immersion (Perceived Control and Interactivity):**
Both versions scored highly in terms of perceived control and engagement. In the PC version, most participants reported firm control (8 positive, 2 negative) and good engagement (6 positive, 2 negative). The VR version had even higher ratings, with nearly all participants experiencing strong control (9 positive, 1 negative) and engagement (10 positive, 1 negative).

**Dynamic Scenario Generation:**
The dynamic scenario generation was positively received in both the PC (8 positive, one negative) and the VR (9 positive, no negatives) versions, highlighting users' appreciation of the tool's interactive features and customizability.

**Realism:**
Realism assessments varied by platform. The PC version's realism received mixed opinions (5 positive, 3 negative), indicating variability in users' perceptions of visual realism. Conversely, the VR version had notably high realism ratings (9 positive, one negative), demonstrating strong alignment with user expectations for realistic virtual environments.

**Overall Satisfaction:**
Both PC and VR platforms achieved high overall satisfaction. The PC version received a solidly positive evaluation (6 positive, no negatives), while the VR version showed even higher satisfaction (8 positive, one negative), indicating strong acceptance and potential for future professional applications.

#### Talk-Aloud Protocol and Group Discussions

The usability and user engagement evaluation for the Immersive Ocean involved technology-proficient participants who assessed the PC and VR versions. Participants reported positive experiences, highlighting strong engagement, intuitive interactions, and effective immersive features across both platforms.

Quantitative analysis from post-questionnaires indicates high usability, especially in eases of navigation and dynamic procedural visualization capabilities. The VR version is particularly praised for immersion and realism, although some navigation controls were challenging, suggesting a learning curve. The PC version is valued for precise control and interface stability, although user interface consistency required improvements.

Qualitative analysis of think-aloud protocols and group discussions reinforced these findings, emphasizing the platform's strengths in interactivity and realism, alongside notable concerns regarding navigation complexity, feedback consistency, and ecological realism. Participants recommended enhancements such as improved navigational aids, clearer interface elements, smoother transitions, and greater ecological detail to elevate user experience and professional applicability.

Overall, these preliminary results affirm the Immersive Ocean's potential as an engaging and intuitive tool for MSP, highlighting areas for targeted improvement to optimize usability and realism.

### 6.3 User Testing Discussion

The user testing revealed that participants generally found the Immersive Ocean highly engaging and intuitive across both PC and VR platforms. The VR version particularly excelled in creating immersive and interactive experiences, notably through spatial interactions and tactile feedback, whereas the PC version demonstrated greater suitability for precise, data-intensive tasks inherent to professional maritime spatial planning.

Despite overall positive feedback, several areas for improvement were identified. Both platforms faced challenges related to navigation, with underwater orientation described as disorienting due to limited spatial references. Users recommended incorporating navigational aids such as mini-maps, compasses, and consistent transition effects between environments. Additionally, the user interface (UI) presented inconsistencies, with menus sometimes behaving unpredictably and data visualizations requiring clearer legends and advanced layer management features.

Dynamic procedural generation was strongly validated as valuable for enhancing spatial planning workflows by providing intuitive, real-time visualizations. Nevertheless, participants noted ecological realism in underwater environments as limited, suggesting enhancements such as more varied marine life and detailed ecological features to improve user immersion and professional relevance.

Overall, participants affirmed the Immersive Ocean's significant potential as an engaging, intuitive, and effective decision-support tool for MSP, emphasizing the importance of addressing identified usability and realism improvements to fully realize its professional applicability.

### 6.4 User Testing Limitations

The user testing conducted with the Immersive Ocean presented several limitations that impact the generalizability of findings. Primarily, the study involved young, technology-proficient participants unfamiliar with MSP, which limited insights into the professional applicability for domain experts. Additionally, testing occurred in a controlled environment, which may have potentially overestimated the ease of use compared to real-world scenarios. Variations in hardware specifications between PC (minimum requirements) and VR (recommended requirements) versions likely influenced user experience outcomes, complicating direct comparisons.

Finally, the qualitative analysis relied on transcription and coding by a single researcher, introducing potential observer bias. While this approach reduced variability in interpretation, it introduced the potential for observer bias in identifying and coding themes. To minimize this bias, the study implemented a structured interviewing protocol centred on predefined categories of interest, ensuring that participant discussions remained relevant to the study's objectives. Additionally, the use of software tools such as Atlas.ti enabled systematic coding and identification of recurring themes while allowing the researcher to capture any emergent topics.

While the limitations identified highlight areas that require further improvement, the mitigation strategies implemented during the research process enhance the validity of the findings. Complementary studies involving professionals in real-world testing environments and iterative evaluations are essential to fully realize the tool's potential.

---

## 7. Conclusions

The introduction of the Immersive Ocean marks a significant advancement in DTO for MSP and Ocean Governance by addressing the interactivity, realism, and usability limitations of existing solutions. Performance validation demonstrated that the implemented technology is effective. It revealed robust stability and rendering efficiency, ensuring seamless user experiences across both PC and VR platforms and confirming scalability and procedural adaptability. Both versions showed acceptable performance in terms of frame rates, resource utilization, and system stability, with the VR version achieving slightly better real-time responsiveness due to its hardware advantage. The virtual twin's interoperability and real-time data integration were successfully validated, confirming that its architectural design supports dynamic updates and complex simulations, making it reliable for its intended use.

User experience testing further proved that the platform's usability is more than adequate, with room and directions for improvement and further development. The evaluations validated the effectiveness of the Immersive Ocean platform, emphasizing its strong intuitive design, interactive realism, and stakeholder engagement capabilities. For tech-savvy young professionals with prior exposure to immersive technologies, the Immersive Ocean is accessible and intuitive. While initial navigation and inconsistent menu behaviours—particularly in VR—posed challenges, participants reported high levels of usability once they were familiar with the controls. PC users benefited from precise navigation mechanics, whereas VR users appreciated the immersive and interactive engagement with the 3D environment. Nevertheless, identified areas such as enhanced ecological detail, consistent navigation mechanisms, and improved interface usability underscore opportunities for refinement.

Thus, while the Immersive Ocean has demonstrated strong usability for digitally literate users, ensuring broader accessibility across diverse user groups—policy-makers, experts, and stakeholders in particular—will require iterative refinements, particularly in guidance functionalities, interface consistency, and potentially stakeholder-specific features.

Immersive Ocean presents a compelling case as a Virtual Twin specifically suited for MSP. Participants unanimously recognized its potential to enhance planning processes through greater interactivity and realism. The dynamic procedural generation of 3D environments from 2D geospatial data was highlighted as a transformative feature, allowing stakeholders to visualize and assess the implications of proposed offshore infrastructure with minimal manual input. The integration of real-time data—such as weather forecasts and ecological parameters—further reinforced its applicability in professional contexts. VR-specific affordances, including tactile interaction and spatial immersion, were seen as particularly effective for stakeholder communication and collaborative decision-making, while the PC version offered greater precision and stability for data-intensive tasks.

Regarding future paths, the next phase of the Immersive Ocean development will focus on expanding user testing to a broader range of stakeholders, with a particular emphasis on MSP professionals and stakeholders, as well as decision-makers in the Blue Economy, including offshore energy and shipping. This transition from novice user validation to expert-policy-maker-stakeholder evaluation is essential to ensure that the Immersive Ocean aligns with the practical needs and decision-making processes within real-world MSP scenarios. Future evaluations will assess the tool's professional applicability by examining how marine/maritime science experts, sectoral stakeholders, and MSP professionals interact with the Immersive Ocean, how well it integrates into or complements their workflows, and how effectively it supports complex spatial planning decisions.

The platform is currently in the early stages of application within real-world case studies involving policymakers and domain experts, including scenarios related to offshore wind farm planning, floating wind farm design and engineering, and environmental impact assessments on turbidity and bird migration.

---

## Acknowledgements

This project has received funding from the European Commission's Horizon 2020 Research and Innovation Program under grant agreement no. 101037643—ILIAD ("Integrated Digital Framework for Comprehensive Maritime Data and Information Services"). The content of this publication reflects only the authors' views, and the European Commission is not responsible for any use that may be made of the information it contains.

**Key contributors to the development of the Immersive Ocean:**
- Kevin Hutchinson-Lhuissier
- Niels Voskens
- Phil de Groot
- Jens Hagen
- Marin Hekman
- Alex Buggenum
- Carlos Pereira Santos
- Davy Goebel
- Joey Relouw

---

## References

[1] ILIAD Project. Integrated Digital Framework for Comprehensive Maritime Data and Information Services. Horizon 2020 Grant Agreement No. 101037643. https://ocean-twin.eu/

[2] European Digital Twin of the Ocean (EDITO). https://www.edito.eu/

[3] Brönner, U., M. Sonnewald, and M. Visbeck, Digital Twins of the Ocean can foster a sustainable blue economy in a protected marine environment. International Hydrographic Review, 2023. 29(1): p. 26-40.

[4] Haupt, S., et al., Exploring the Use of Data in a Digital Twin for the Marine and Coastal Environment. ISPRS International Journal of Geo-Information, 2025. 14(4): p. 140.

[5] Wehn, U., et al., Past and present marine citizen science around the globe: A cumulative inventory of initiatives and data produced. Ambio, 2025: p. 1-16.

[6] Schaefer, N. and V. Barale, Maritime spatial planning: opportunities and challenges in the framework of the EU integrated maritime policy. Journal of coastal conservation, 2011. 15: p. 237-245.

[7] Tzachor, A., O. Hendel, and C.E. Richards, Digital twins: a stepping stone to achieve ocean sustainability? npj Ocean Sustainability, 2023. 2(1): p. 16.

[8] Lilja Bye, B., A.-J. Berre, and U. Brönner. Interoperable digital twins of the ocean through aligned architectures. in EGU General Assembly Conference Abstracts. 2023.

[9] Zeneli, M. and G. Marinova. Navigating the Future: Digital Twin in Maritime Industry. in 2024 International Conference on Broadband Communications for Next Generation Networks and Multimedia Applications (CoBCom). 2024. IEEE.

[10] Chen, G., et al., Toward digital twin of the ocean: From digitalization to cloning. Intelligent Marine Technology and Systems, 2023. 1(1): p. 3.

[11] Miedtank, A., et al., Marine digital twins for enhanced ocean understanding. Remote Sensing Applications: Society and Environment, 2024. 36: p. 101268.

[12] Lee, J.-H., et al., Real-time digital twin for ship operation in waves. Ocean Engineering, 2022. 266: p. 112867.

[13] Bi, J., et al., Research on the construction of a digital twin system for the long-term service monitoring of port terminals. Journal of Marine Science and Engineering, 2024. 12(7): p. 1215.

[14] Kwon, Y.-S., et al., Development and Validation of a Digital Twin Vessel Based on an Unmanned Surface Vehicle. Journal of Marine Science and Engineering, 2024. 12(4): p. 568.

[15] Zhang, J., et al., Towards the digital twin in the marine industry: A survey on data acquisition, processing, and applications. Ocean Engineering, 2024. 298: p. 117123.

[16] Kousiouris, G., et al., Cloud-based digital twin framework for vessel structural health monitoring. Ocean Engineering, 2023. 274: p. 114083.

[17] Fonseca, X., et al., Maritime Spatial Planning - What are the perspectives of young adults? in 13th annual International Conference of the International Institute on Informatics and Systemics. 2019.

[18] Fonseca, X., et al., The impact of stakeholder engagement in maritime spatial planning: The case study of Portugal. Marine Policy, 2024. 165: p. 106196.

[19] Mayer, I., et al., The MSP Challenge simulation platform: From serious games to policy support tools. Journal of Environmental Assessment Policy and Management, 2022. 24(02n03).

[20] Soma, K., et al., Stakeholder participation in maritime spatial planning: A study on the North Sea and the Baltic Sea. Marine Policy, 2021. 132: p. 104655.

[21] Pai, S., et al., The Role of Serious Gaming in Maritime Spatial Planning and Policy Formulation: The BONUS BALTSPACE Project Approach. in Trans-Atlantic Cooperation in Maritime Spatial Planning. 2024. Springer.

[22] Fonseca, X., et al., Designing immersive and interactive serious games for maritime spatial planning using the MSP Challenge simulation platform. Environmental Development, 2024. 50: p. 100958.

[23] Jay, S., et al., International progress in marine spatial planning. Ocean Yearbook Online, 2012. 26(1): p. 171-212.

[24] Fonseca, X., et al., Exploring the use of serious games and gamification to support maritime spatial planning stakeholder engagement and collaborative learning: Review of research and available tools. Environmental Development, 2022. 43: p. 100746.

[25] Visbeck, M., Ocean sustainability and digital twins. Nature Sustainability, 2023. 6(12): p. 1471-1471.

[26] Raoux, A., et al., Evaluating ecosystem services and their spatial variations in the North Sea. Marine Policy, 2020. 121: p. 104154.

[27] Hennen, L., M. Schradi, and A. Banse, Governance of the EU's digital twin of the ocean. Frontiers in Marine Science, 2024. 11: p. 1339077.

[28] Fonseca, X., et al., Enhancing stakeholder engagement and sustainable blue growth in maritime spatial planning through serious gaming and immersive technologies. Frontiers in Marine Science, 2024. 11: p. 1421683.

[29] Santos, C.P., et al., Digital Twins and the Baltic Sea—A Bibliometric Analysis. Sustainability, 2023. 15(16): p. 12281.

[30] Zhang, L., et al., The Current Status of Serious Games in Marine Spatial Planning. Environmental Development, 2024. 50: p. 100944.

[31] De Groot, P. and X. Fonseca, The Immersive Ocean: An Innovative Approach to Digital Twin Development in Maritime Spatial Planning. in International Conference on Games and Learning Alliance. 2024. Springer.

[32] Immersive Ocean, Iliad marketplace. https://ocean-twin.eu/marketplace/product/proceduralocean

[33] Unreal Engine. https://www.unrealengine.com/

[34] Maritime Spatial Planning Challenge. https://www.mspchallenge.info/

[35] Open-Meteo Free Weather API. https://open-meteo.com/

[36] Open Geospatial Consortium Publications. https://www.ogc.org/publications/

[37] ETRS89-extended/LAEA Europe (EPSG:3035). https://epsg.io/3035

[38] The GeoJSON IETF Standard Specification (RFC 7946). https://datatracker.ietf.org/doc/html/rfc7946

[39] QGIS Open-Source Platform. https://qgis.org/

[40] ArcGIS Online. https://www.arcgis.com/

[41] EMODnet. https://emodnet.ec.europa.eu/

[42] Copernicus. https://www.copernicus.eu/

[43] European Digital Twin Ocean – EDITO. https://www.edito.eu/

[44] Atlas.ti Qualitative Data Analysis Software. https://atlasti.com/

---

## Contact Information

**Project:** Immersive Ocean - ILIAD Digital Twin of the Ocean

**Institution:** Breda University of Applied Sciences
Academy for AI, Games and Media
Breda, Netherlands

**Primary Contact:** Xavier Fonseca (santosfonseca.f@buas.nl)

---
