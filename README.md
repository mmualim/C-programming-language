**Project Summary**
This project is a C program designed to process trajectory data to help with contact tracing by analyzing movement records. It calculates distances from home locations, identifies close contacts, and visualizes travel distances. The main stages are:

1. Read and Process the First Record
- Reads the first line of data to print out key details: user ID, visited location, visit start and end times, and distance from home.
- Uses the haversine formula to calculate the distance between two points (visited location and home location).
Process All Records with Distance Calculation

2. Process All Records with Distance Calculation
- Processes each line in the dataset, calculating and displaying the distance from home for each location.
- Visualizes the distances in a simple bar format, providing a quick way to interpret proximity from home.
Detect Close Contacts

3. Detect close contacts
- Checks for "close contacts" by finding pairs of users who were within 2 meters of each other for over 15 minutes.
- Outputs the total number of close-contact pairs and highlights the pair with the longest overlapping time.
Optimize Close Contact Detection (Optional)

4. Optimize Close Contact Detection
- Suggests methods for making the close-contact detection process more efficient by reducing unnecessary checks.
- This stage is for additional optimization but is optional.

Each stage builds functionality step-by-step, moving from basic data handling to more complex analysis and visual representation. This structure makes it easy to understand and expand on each part of the program as it progresses.
