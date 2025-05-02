from flask import Flask, jsonify, request
import datetime
import time

app = Flask(__name__)

# --- Mock Data ---
# Keep track of data (in memory, resets when server restarts)
mock_employees = {
    1: {"id": 1, "name": "Alice Manager (Mock)", "role": "Manager", "reportsTo": None, "isActive": True, "hireDate": "2023-01-15T10:00:00Z"},
    2: {"id": 2, "name": "Bob Boss (Mock)", "role": "Boss", "reportsTo": 1, "isActive": True, "hireDate": "2023-02-20T11:30:00Z"},
    3: {"id": 3, "name": "Charlie Specialist (Mock)", "role": "Specialist", "reportsTo": 2, "isActive": True, "hireDate": "2023-03-10T09:15:00Z"}
}
mock_reviews = {
    101: {"reviewId": 101, "employeeId": 2, "reviewerId": 1, "reviewDate": "2024-05-01T14:00:00Z", "overallRating": 4.0, "comments": "Good progress (Mock)", "punctuality": 5},
    102: {"reviewId": 102, "employeeId": 3, "reviewerId": 2, "reviewDate": "2024-05-02T10:30:00Z", "overallRating": 3.5, "comments": "Needs improvement on planning (Mock)", "adequacyPlanning": 2}
}
next_employee_id = 4
next_review_id = 103

# --- API Endpoints ---

@app.route('/')
@app.route('/health')
def health_check():
    return jsonify({"status": "OK", "message": "Mock server is running"}), 200

@app.route('/api/employees', methods=['GET'])
def get_employees():
    print("GET /api/employees requested")
    # Return list of all employee values
    return jsonify(list(mock_employees.values())), 200

@app.route('/api/employees/<int:employee_id>', methods=['GET'])
def get_employee(employee_id):
    print(f"GET /api/employees/{employee_id} requested")
    employee = mock_employees.get(employee_id)
    if employee:
        return jsonify(employee), 200
    else:
        return jsonify({"error": "Employee not found"}), 404

@app.route('/api/employees', methods=['POST'])
def add_employee():
    global next_employee_id
    print("POST /api/employees requested")
    if not request.is_json:
        return jsonify({"error": "Request must be JSON"}), 400

    data = request.get_json()
    print(f"Received data: {data}")

    # Basic validation (add more as needed)
    if not data or 'name' not in data or 'role' not in data:
         return jsonify({"error": "Missing required fields (name, role)"}), 400

    new_employee = {
        "id": next_employee_id,
        "name": data.get("name"),
        "role": data.get("role"),
        "reportsTo": data.get("reportsTo"), # Assumes client sends ID or null
        "isActive": data.get("isActive", True),
        "hireDate": data.get("hireDate", datetime.datetime.now(datetime.timezone.utc).isoformat(timespec='seconds') + 'Z')
    }
    mock_employees[next_employee_id] = new_employee
    print(f"Added employee: {new_employee}")
    response_data = new_employee # Respond with the created object including ID
    next_employee_id += 1
    return jsonify(response_data), 201 # 201 Created status

@app.route('/api/employees/<int:employee_id>', methods=['PUT'])
def update_employee(employee_id):
    print(f"PUT /api/employees/{employee_id} requested")
    if employee_id not in mock_employees:
         return jsonify({"error": "Employee not found"}), 404
    if not request.is_json:
        return jsonify({"error": "Request must be JSON"}), 400

    data = request.get_json()
    print(f"Received data: {data}")

    # Update existing employee (only fields provided in request)
    employee = mock_employees[employee_id]
    employee["name"] = data.get("name", employee["name"])
    employee["role"] = data.get("role", employee["role"])
    employee["reportsTo"] = data.get("reportsTo", employee["reportsTo"])
    employee["isActive"] = data.get("isActive", employee["isActive"])
    employee["hireDate"] = data.get("hireDate", employee["hireDate"])

    mock_employees[employee_id] = employee # Update in our mock store
    print(f"Updated employee {employee_id}: {employee}")
    return jsonify(employee), 200 # 200 OK

@app.route('/api/reviews', methods=['GET'])
def get_reviews():
    # Example: Support filtering by employeeId
    employee_id = request.args.get('employeeId', type=int)
    print(f"GET /api/reviews requested (employeeId={employee_id})")

    if employee_id is not None:
        filtered_reviews = [r for r in mock_reviews.values() if r.get("employeeId") == employee_id]
        return jsonify(filtered_reviews), 200
    else:
        # Return all reviews if no filter specified (or handle as error)
        return jsonify(list(mock_reviews.values())), 200


@app.route('/api/reviews', methods=['POST'])
def add_review():
    global next_review_id
    print("POST /api/reviews requested")
    if not request.is_json:
        return jsonify({"error": "Request must be JSON"}), 400

    data = request.get_json()
    print(f"Received data: {data}")

    # Basic validation
    if not data or 'employeeId' not in data or 'reviewerId' not in data:
         return jsonify({"error": "Missing required fields (employeeId, reviewerId)"}), 400

    # Ensure related employees exist (optional check)
    # if data['employeeId'] not in mock_employees or data['reviewerId'] not in mock_employees:
    #    return jsonify({"error": "Employee or Reviewer not found"}), 400

    new_review = {
        "reviewId": next_review_id,
        "employeeId": data.get("employeeId"),
        "reviewerId": data.get("reviewerId"),
        "reviewDate": data.get("reviewDate", datetime.datetime.now(datetime.timezone.utc).isoformat(timespec='seconds') + 'Z'),
        "overallRating": data.get("overallRating"),
        "comments": data.get("comments"),
        "punctuality": data.get("punctuality"),
        "conflictManagement": data.get("conflictManagement"),
        "adequacyPlanning": data.get("adequacyPlanning"),
        "taskExecutionSpeed": data.get("taskExecutionSpeed")
    }
    mock_reviews[next_review_id] = new_review
    print(f"Added review: {new_review}")
    response_data = new_review # Respond with created object
    next_review_id += 1
    return jsonify(response_data), 201 # 201 Created


# --- Run the Server ---
if __name__ == '__main__':
    # Run on localhost (127.0.0.1) port 5000
    # Accessible only from your machine
    print("Starting mock server on http://127.0.0.1:5000")
    app.run(host='127.0.0.1', port=5000, debug=True) # debug=True helps see errors