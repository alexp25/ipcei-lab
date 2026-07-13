---
title: "LP5 - Project Documentation"
description: "Generating a project documentation draft with Agent 2 and turning it into a finished Markdown report"
nav_order: 6
parent: Additional Demos
layout: lesson
---

# LP5 - Project Documentation

**From scoping brief and requirements report to a reviewable Markdown project documentation file**

---

| | |
|---|---|
| **Session** | Summer project documentation |
| **Board** | FRDM-MCXA153 |
| **Tooling** | Coding agent prompt, Markdown editing in VS Code, Markdown preview extensions |
| **Agent prompt** | `src/lab_new_project_configured/configured_project/.agents/agent_2_project_documentation_markdown_generator.md` |
| **Inputs** | `.agents/data/project_scoping_brief.json`, `.agents/data/requirements_report.html` |
| **Output** | `.agents/data/project_documentation.md` |
| **Goal** | Turn Agent 0 and Agent 1 outputs into a single Markdown project report, then finish it by hand |

## Context

**LP4 - Project Setup** produced two artifacts for your summer project:

1. the Agent 0 scoping brief (`project_scoping_brief.json` / `.html`);
2. the Agent 1 requirements report (`requirements_report.html`).

Those are working documents for scoping and requirements review. They are not the final project report. **Agent 2: Project Documentation Markdown Generator** reads both files and drafts a single, self-contained **Markdown** file that follows the structure normally expected from a university embedded systems project report: introduction, general description, hardware design, software design, results, conclusions, and so on.

The prompt file is here:

```text
src/lab_new_project_configured/configured_project/.agents/
  agent_2_project_documentation_markdown_generator.md
  agent_2_invocation_prompt.txt
  data/
```

As with Agent 0 and Agent 1, the output is a **draft**. Agent 2 is explicitly instructed to separate confirmed facts from AI assumptions, mark unresolved decisions, and leave `TODO` / `TBD` placeholders wherever information is missing. You still have to read it, correct it, fill in the placeholders, and get it approved.

## Workflow

1. `.agents/data/project_scoping_brief.json` + `.agents/data/requirements_report.html` → **Agent 2** → `.agents/data/project_documentation.md`
2. Open `project_documentation.md` in VS Code and review it section by section.
3. Fill in the `TODO` placeholders as the project progresses (results, journal entries, bibliography, final schematics).
4. Keep the file in the project repository as the living project report.

Agent 2 does not invent hardware details it wasn't given. It keeps exact MCU pins, SDK API names, and numeric thresholds out of the document unless Agent 0 or Agent 1 already provided them, and it uses generic pin-capability language (GPIO-capable, ADC-capable, PWM-capable, I2C/UART/SPI-capable) instead.

## Step 1: Generate the Documentation with Agent 2

Make sure `.agents/data/project_scoping_brief.json` and `.agents/data/requirements_report.html` already exist from LP4 before running this step.

Use the following invocation prompt (also saved in `.agents/agent_2_invocation_prompt.txt`).

<details class="prompt-code" markdown="1">
<summary><strong>Prompt: Invoke Agent 2</strong></summary>

<div markdown="1">

````markdown
Use `.agents/agent_2_project_documentation_markdown_generator.md`.

This is the Agent 2: Project Documentation Markdown Generator prompt.

Agent 0 JSON file:

<<<SCOPING_JSON_FILE>>>
.agents/data/project_scoping_brief.json
<<<END_SCOPING_JSON_FILE>>>

Agent 1 requirements HTML file:

<<<REQUIREMENTS_HTML_FILE>>>
.agents/data/requirements_report.html
<<<END_REQUIREMENTS_HTML_FILE>>>

Optional Agent 0 scoping HTML file:

<<<SCOPING_HTML_FILE>>>
.agents/data/project_scoping_brief.html
<<<END_SCOPING_HTML_FILE>>>

Output Markdown file:

<<<OUTPUT_MARKDOWN_PATH>>>
.agents/data/project_documentation.md
<<<END_OUTPUT_MARKDOWN_PATH>>>

Read the provided input files and generate the final project documentation Markdown file according to the Agent 2 prompt.

Create or overwrite the output Markdown file.

After writing the file, respond only with the output Markdown file path and a one-sentence status.
````

</div>
</details>

The agent will write (or overwrite) `.agents/data/project_documentation.md`. It should reply with just the output path and a one-sentence status — the interesting part is the file itself, not the chat response.

## Step 2: What the Generated Document Contains

The generated Markdown follows a fixed structure so every report looks the same shape:

1. Introduction
2. General description (project summary, feature tiers, scenarios, user stories, use case diagram, hardware/software block diagram)
3. Hardware design (BOM, hardware block diagram description, pin allocation draft, schematic placeholders, signal/measurement placeholders)
4. Software design (development environment, firmware architecture, algorithms, functional/non-functional requirements summaries, test plan summary, traceability summary)
5. Risk matrix
6. Assumptions and open questions
7. Human review checklist
8. Obtained results (`TODO`, filled in after implementation)
9. Conclusions (`TODO`, filled in at the end)
10. Download (links/attachments placeholder)
11. Project journal (starter table)
12. Bibliography / resources
13. Documentation status (`READY FOR HUMAN REVIEW`, `READY FOR LOW-RISK PROTOTYPE ONLY`, or `NOT READY - MISSING CRITICAL INFORMATION`)

Diagrams (use case, hardware/software block diagram) are generated as **Mermaid** code blocks, which render directly in GitHub, GitLab, and VS Code's Markdown preview — no separate diagram tool needed.

## Step 3: Review the Draft

Before treating the document as anything other than a draft, check:

1. the project name, summary, and objective match what you actually intend to build;
2. `TODO` markers are still present where information is genuinely missing — do not let the agent (or yourself) quietly invent pins, SDK calls, or numeric thresholds;
3. the feature tiers (core / recommended / advanced) match the scope you agreed on in LP4;
4. the risk matrix includes the mandatory checks: 3.3 V compatibility, motor/power current, missing stop behavior, firmware timing, and incomplete datasheet review;
5. the Human Review Checklist (Section 7) is something your instructor can actually sign off on;
6. the Mermaid diagrams render correctly and describe your real hardware, not a generic example.

## Step 4: Work on the Markdown File Directly

From this point on, `project_documentation.md` is a normal file you edit by hand in VS Code, the same way you edit code:

- Fill in `TODO: Complete after implementation.` once you have working results, photos, and measurements.
- Replace `TBD` component names, quantities, and pins once hardware is selected and the `.mex` configuration is final.
- Add rows to the **Project Journal** table (Section 11) as you go, instead of trying to reconstruct the timeline at the end.
- Add a final schematic image, a demo video link, or a repository link under **Download** (Section 10) when they exist.
- Update **Documentation Status** (Section 13) as the project matures, from `NOT READY` to `READY FOR LOW-RISK PROTOTYPE ONLY` to `READY FOR HUMAN REVIEW`.

Treat AI-authored sections the same way you'd treat a teammate's first draft: useful, but not final until you've checked it against the real hardware and the real schedule.

## Step 5: Install a Markdown Preview Extension

A 600+ line Markdown file with tables and Mermaid diagrams is hard to read as raw text. Install a preview extension so you can toggle between source and rendered view while editing.

1. Open the Extensions view in VS Code (`Ctrl+Shift+X`).
2. Search for **Markdown View Toggle**.
3. Click **Install**.

Once installed, open `project_documentation.md` and use the extension's toggle command (or VS Code's built-in **Open Preview** button in the editor title bar / `Ctrl+Shift+V`) to switch between the raw Markdown source and the rendered view. The rendered view is what shows tables, checklists, and Mermaid diagrams the way they will look on GitHub or GitLab.

Keep both views in mind while editing:

- edit in the **source** view for text, tables, and `TODO` replacements;
- switch to the **preview** view to confirm tables are not broken and Mermaid diagrams still render after an edit.

## Step 6: Publish the Project on GitHub

Each summer project needs its own **GitHub repository**, and the documentation file you just generated and reviewed becomes that repository's `README.md`.

1. Create a new GitHub repository for the project (public or private, per your instructor's rules).
2. Copy or rename the generated file so it sits at the repository root as `README.md`:

   ```text
   project_documentation.md  ->  README.md
   ```

3. Commit and push it together with the rest of the project (firmware source, `.mex` configuration, `.agents/data/` inputs).
4. Open the repository page on GitHub and confirm the README renders correctly — tables, checkboxes, and Mermaid diagrams all render natively in GitHub's Markdown viewer, so this is also a good final check that nothing broke during manual edits.

Keep updating `README.md` directly as the project progresses (results, journal entries, final status), the same way you were editing `project_documentation.md` locally. The GitHub repository, with this file as its README, is the artifact that gets submitted and reviewed at the end of the summer project.

## Deliverable

Keep in your project folder and repository:

1. the Agent 0 and Agent 1 input files (JSON scoping brief, HTML scoping report, HTML requirements report);
2. the generated documentation file, published as `README.md` in the project's GitHub repository;
3. your edits to that file as the project progresses (results, journal, bibliography, final status);
4. a short human review note listing what you accepted, corrected, or rejected from the AI-generated draft.

This Markdown file becomes the project's living documentation and its GitHub README, from the initial draft through implementation to the final demo and conclusions.
