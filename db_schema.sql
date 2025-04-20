-- Table to store employee information
CREATE TABLE IF NOT EXISTS Employees (
    EmployeeID INTEGER PRIMARY KEY AUTOINCREMENT,
    Name TEXT NOT NULL,
    Role TEXT NOT NULL CHECK(Role IN ('Manager', 'Boss', 'Developer', 'Technician')),
    ReportsTo INT,
    HireDate DATE,
    IsActive INT DEFAULT 1,
    FOREIGN KEY (ReportsTo) REFERENCES Employees(EmployeeID)
);

-- Table to store performance review data
CREATE TABLE IF NOT EXISTS PerformanceReviews (
    ReviewID INTEGER PRIMARY KEY AUTOINCREMENT,
    EmployeeID INT NOT NULL,
    ReviewerID INT NOT NULL,
    ReviewDate DATE NOT NULL DEFAULT CURRENT_DATE,
    OverallRating REAL, -- Optional overall score  
    Comments TEXT,

    -- Key Performance Indicators (KPI) range 1 to 10
    PunctualityRating REAL CHECK(PunctualityRating BETWEEN 1 AND 10),
    QualityOfWorkRating REAL CHECK(QualityOfWorkRating BETWEEN 1 AND 10),
    TeamworkRating REAL CHECK(TeamworkRating BETWEEN 1 AND 10),
    CommunicationRating REAL CHECK(CommunicationRating BETWEEN 1 AND 10),
    ProblemSolvingRating REAL CHECK(ProblemSolvingRating BETWEEN 1 AND 10),
    CreativityRating REAL CHECK(CreativityRating BETWEEN 1 AND 10),
    TechnicalSkillsRating REAL CHECK(TechnicalSkillsRating BETWEEN 1 AND 10),
    AdaptabilityRating REAL CHECK(AdaptabilityRating BETWEEN 1 AND 10),
    LeadershipRating REAL CHECK(LeadershipRating BETWEEN 1 AND 10),
    InitiativeRating REAL CHECK(InitiativeRating BETWEEN 1 AND 10),
    
    -- Optional fields for additional comments

    FOREIGN KEY (EmployeeID) REFERENCES Employees(EmployeeID),
    FOREIGN KEY (ReviewerID) REFERENCES Employees(EmployeeID)
);

-- Optional indexes for faster lookups
CREATE INDEX IF NOT EXISTS idx_employee_name ON Employees(Name);
CREATE INDEX IF NOT EXISTS idx_employee_role ON Employees(Role);
CREATE INDEX IF NOT EXISTS idx_review_employee_id ON PerformanceReviews(EmployeeID);
CREATE INDEX IF NOT EXISTS idx_review_reviewer_id ON PerformanceReviews(ReviewerID);
CREATE INDEX IF NOT EXISTS idx_review_date ON PerformanceReviews(ReviewDate);
