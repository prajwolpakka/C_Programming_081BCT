# C Programming Project Submission

Welcome to the **C Programming Project Repository (081BCT)**. Each team is required to submit their project by following the instructions below.

## 📌 Submission Instructions

1. **One team member forks this repository** into their GitHub account (Team Lead):
   - Go to the instructor's repository: [C_Programming_081BCT](https://github.com/prajwolpakka/C_Programming_081BCT).
   - Click the **Fork** button at the top-right corner to create a copy under your GitHub account.

2. **The entire team clones the forked repository**:
   ```bash
   git clone https://github.com/TEAM_LEAD_USERNAME/C_Programming_081BCT.git
   ```

3. **Create a new branch** for your team using your roll numbers in ascending order, formatted as two-digit numbers.  
   - Example: If the team consists of roll numbers **1, 4, 5, 45**, the branch name should be:
     ```
     01040545
     ```
   - Create the branch using the following command:
     ```bash
     git checkout -b 01040545
     ```

4. **Make changes in your cloned repository** by adding project files inside the respective folders:
   - `proposal/` → Your project proposal document.
   - `code/` → Your source code files.
   - `report/` → Your final report.
   - `slide/` → Presentation slides.

5. **Commit and push changes** to the forked repository:
   ```bash
   git add .
   git commit -m "Added project submission for team 01040545"
   git push origin 01040545
   ```

6. **The Team Lead opens a Pull Request (PR)** from their forked repository to the `main` branch of this repository.
7. **Submit the PR link** to the instructor for review.

## 📂 Folder Structure
Ensure your files are placed in the correct folders:
```
C_Programming_081BCT/
│-- proposal/
│   ├── team_project_proposal.pdf
│-- code/
│   ├── main.c
│   ├── additional_files.c
│-- report/
│   ├── team_project_report.pdf
│-- slide/
│   ├── team_presentation.pptx
```

## ❗ Important Notes
- Each team should submit only **one PR** per group.
- The **branch name must follow the roll number format** to avoid confusion.
- Keep your repository **private** until the submission deadline.
- Ensure your code is well-documented and formatted correctly.

For any questions, contact the instructor. Happy coding! 🚀