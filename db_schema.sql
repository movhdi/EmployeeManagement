-- Table to store employee information
CREATE TABLE IF NOT EXISTS employees (
              employee_id INTEGER PRIMARY KEY AUTOINCREMENT,
              name TEXT NOT NULL,
              role TEXT NOT NULL CHECK(role IN ('Manager', 'Boss', 'Specialist', 
              'Technician')),
              reports_to INT,
              hire_date TEXT,
              personnel_code INT,
              is_active INT DEFAULT 1,
              FOREIGN KEY (reports_to) REFERENCES employees(employee_id)
              );

-- Table to store performance review data
CREATE TABLE IF NOT EXISTS performance_reviews (
        review_id INTEGER PRIMARY KEY AUTOINCREMENT,
        employee_id INTEGER NOT NULL,
        reviewer_id INTEGER NOT NULL,
        review_date DATE NOT NULL DEFAULT CURRENT_DATE,
        overall_rating REAL,
        comments TEXT,

        -- Key Performance Indicators (KPI) range 1 to 10
        punctuality_rating REAL CHECK(punctuality_rating BETWEEN 1 AND 10),
        quality_of_work_rating REAL CHECK(quality_of_work_rating BETWEEN 1 AND 
        10),
        teamwork_rating REAL CHECK(teamwork_rating BETWEEN 1 AND 10),
        communication_rating REAL CHECK(communication_rating BETWEEN 1 AND 
        10),
        problem_solving_rating REAL CHECK(problem_solving_rating BETWEEN 1 AND 
        10),
        creativity_rating REAL CHECK(creativity_rating BETWEEN 1 AND 10),
        Technical_skills_rating REAL CHECK(technical_skills_rating BETWEEN 1 
        AND 10),
        adaptability_rating REAL CHECK(adaptability_rating BETWEEN 1 AND 10),
        leadership_rating REAL CHECK(leadership_rating BETWEEN 1 AND 10),
        initiative_rating REAL CHECK(initiative_rating BETWEEN 1 AND 10),

        FOREIGN KEY (employee_id) REFERENCES employees(employee_id),
        FOREIGN KEY (reviewer_id) REFERENCES employees(employee_id)
);

CREATE INDEX IF NOT EXISTS idx_employee_name ON employees(name);
CREATE INDEX IF NOT EXISTS idx_employee_role ON employees(role);
CREATE INDEX IF NOT EXISTS idx_review_employee_id ON 
performance_reviews(employee_id);
CREATE INDEX IF NOT EXISTS idx_review_reviewer_id ON
performance_reviews(reviewer_id);
CREATE INDEX IF NOT EXISTS idx_review_date ON
performance_reviews(review_date);