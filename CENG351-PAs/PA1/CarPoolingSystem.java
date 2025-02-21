package ceng.ceng351.carpoolingdb;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class CarPoolingSystem implements ICarPoolingSystem {

    private static String url = "jdbc:h2:mem:carpoolingdb;DB_CLOSE_DELAY=-1"; // In-memory database
    private static String user = "sa";          // H2 default username
    private static String password = "";        // H2 default password

    private Connection connection;

    public void initialize(Connection connection) {
        this.connection = connection;
    }

    //Given: getAllDrivers()
    //Testing 5.16: All Drivers after Updating the Ratings
    @Override
    public Driver[] getAllDrivers() {
        List<Driver> drivers = new ArrayList<>();

        String query = "SELECT PIN, rating FROM Drivers ORDER BY PIN ASC;";

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                int PIN = rs.getInt("PIN");
                double rating = rs.getDouble("rating");

                // Create a Driver object with only PIN and rating
                Driver driver = new Driver(PIN, rating);
                drivers.add(driver);
            }
            rs.close();
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return drivers.toArray(new Driver[0]);
    }


    //5.1 Task 1 Create tables
    @Override
    public int createTables() {
        int tableCount = 0;

        String createParticipantsTable = "CREATE TABLE IF NOT EXISTS Participants (" +
                "PIN INT PRIMARY KEY, " +
                "p_name VARCHAR(50), " +
                "age INT" +
                ");";

        String createPassengersTable = "CREATE TABLE IF NOT EXISTS Passengers (" +
                "PIN INT PRIMARY KEY, " +
                "membership_status VARCHAR(50), " +
                "FOREIGN KEY (PIN) REFERENCES Participants(PIN)" +
                ");";

        String createDriversTable = "CREATE TABLE IF NOT EXISTS Drivers (" +
                "PIN INT PRIMARY KEY, " +
                "rating DOUBLE, " +
                "FOREIGN KEY (PIN) REFERENCES Participants(PIN)" +
                ");";

        String createCarsTable = "CREATE TABLE IF NOT EXISTS Cars (" +
                "CarID INT PRIMARY KEY, " +
                "PIN INT, " +
                "color VARCHAR(20), " +
                "brand VARCHAR(50), " +
                "FOREIGN KEY (PIN) REFERENCES Drivers(PIN)" +
                ");";

        String createTripsTable = "CREATE TABLE IF NOT EXISTS Trips (" +
                "TripID INT PRIMARY KEY, " +
                "CarID INT, " +
                "date DATE, " +
                "departure VARCHAR(30), " +
                "destination VARCHAR(30), " +
                "num_seats_available INT, " +
                "FOREIGN KEY (CarID) REFERENCES Cars(CarID)" +
                ");";

        String createBookingsTable = "CREATE TABLE IF NOT EXISTS Bookings (" +
                "TripID INT, " +
                "PIN INT, " +
                "booking_status VARCHAR(30), " +
                "PRIMARY KEY(TripID, PIN), " +
                "FOREIGN KEY (TripID) REFERENCES Trips(TripID), " +
                "FOREIGN KEY (PIN) REFERENCES Passengers(PIN)" +
                ");";

        try(Statement statement = this.connection.createStatement()) {

            try {
                statement.executeUpdate(createParticipantsTable);
                tableCount++;
            } catch (SQLException e) {
                System.err.println("Failed to create Participants table: " + e.getMessage());
            }

            try {
                statement.executeUpdate(createPassengersTable);
                tableCount++;
            } catch (SQLException e) {
                System.err.println("Failed to create Passengers table: " + e.getMessage());
            }

            try {
                statement.executeUpdate(createDriversTable);
                tableCount++;
            } catch (SQLException e) {
                System.err.println("Failed to create Drivers table: " + e.getMessage());
            }

            try {
                statement.executeUpdate(createCarsTable);
                tableCount++;
            } catch (SQLException e) {
                System.err.println("Failed to create Cars table: " + e.getMessage());
            }

            try {
                statement.executeUpdate(createTripsTable);
                tableCount++;
            } catch (SQLException e) {
                System.err.println("Failed to create Trips table: " + e.getMessage());
            }

            try {
                statement.executeUpdate(createBookingsTable);
                tableCount++;
            } catch (SQLException e) {
                System.err.println("Failed to create Bookings table: " + e.getMessage());
            }
        } catch (SQLException e) {
            System.err.println("Failed to create statement: " + e.getMessage());
        }

        return tableCount;
    }


    //5.17 Task 17 Drop tables
    @Override
    public int dropTables() {
        int tableCount = 0;

        String deleteBookingsTable = "DROP TABLE IF EXISTS Bookings;";

        String deleteTripsTable = "DROP TABLE IF EXISTS Trips;";

        String deleteCarsTable = "DROP TABLE IF EXISTS Cars;";

        String deleteDriversTable = "DROP TABLE IF EXISTS Drivers;";

        String deletePassengersTable = "DROP TABLE IF EXISTS Passengers;";

        String deleteParticipantsTable = "DROP TABLE IF EXISTS Participants;";

        try(Statement statement = this.connection.createStatement()) {

            try {
                statement.executeUpdate(deleteBookingsTable);
                tableCount++;
            } catch (SQLException e) {
                System.err.println("Failed to delete Bookings table: " + e.getMessage());
            }

            try {
                statement.executeUpdate(deleteTripsTable);
                tableCount++;
            } catch (SQLException e) {
                System.err.println("Failed to delete Trips table: " + e.getMessage());
            }

            try {
                statement.executeUpdate(deleteCarsTable);
                tableCount++;
            } catch (SQLException e) {
                System.err.println("Failed to delete Cars table: " + e.getMessage());
            }

            try {
                statement.executeUpdate(deleteDriversTable);
                tableCount++;
            } catch (SQLException e) {
                System.err.println("Failed to delete Drivers table: " + e.getMessage());
            }

            try {
                statement.executeUpdate(deletePassengersTable);
                tableCount++;
            } catch (SQLException e) {
                System.err.println("Failed to delete Passengers table: " + e.getMessage());
            }

            try {
                statement.executeUpdate(deleteParticipantsTable);
                tableCount++;
            } catch (SQLException e) {
                System.err.println("Failed to delete Participants table: " + e.getMessage());
            }
        } catch (SQLException e) {
            System.err.println("Failed to create statement: " + e.getMessage());
        }

        return tableCount;
    }


    //5.2 Task 2 Insert Participants
    @Override
    public int insertParticipants(Participant[] participants) {
        int rowsInserted = 0;

        String query = "INSERT INTO Participants " +
                "VALUES (?, ?, ?);";

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            for (Participant participant : participants) {
                ps.setInt(1, participant.getPIN());
                ps.setString(2, participant.getP_name());
                ps.setInt(3, participant.getAge());
                rowsInserted += ps.executeUpdate();
            }
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return rowsInserted;
    }


    //5.2 Task 2 Insert Passengers
    @Override
    public int insertPassengers(Passenger[] passengers) {
        int rowsInserted = 0;

        String query = "INSERT INTO Passengers " +
                "VALUES (?, ?);";

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            for (Passenger passenger : passengers) {
                ps.setInt(1, passenger.getPIN());
                ps.setString(2, passenger.getMembership_status());
                rowsInserted += ps.executeUpdate();
            }
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return rowsInserted;
    }


    //5.2 Task 2 Insert Drivers
    @Override
    public int insertDrivers(Driver[] drivers) {
        int rowsInserted = 0;

        String query = "INSERT INTO Drivers " +
                "VALUES (?, ?);";

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            for (Driver driver : drivers) {
                ps.setInt(1, driver.getPIN());
                ps.setDouble(2, driver.getRating());
                rowsInserted += ps.executeUpdate();
            }
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return rowsInserted;
    }


    //5.2 Task 2 Insert Cars
    @Override
    public int insertCars(Car[] cars) {
        int rowsInserted = 0;

        String query = "INSERT INTO Cars " +
                "VALUES (?, ?, ?, ?);";

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            for (Car car : cars) {
                ps.setInt(1, car.getCarID());
                ps.setInt(2, car.getPIN());
                ps.setString(3, car.getColor());
                ps.setString(4, car.getBrand());
                rowsInserted += ps.executeUpdate();
            }
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return rowsInserted;
    }


    //5.2 Task 2 Insert Trips
    @Override
    public int insertTrips(Trip[] trips) {
        int rowsInserted = 0;

        String query = "INSERT INTO Trips " +
                "VALUES (?, ?, ?, ?, ?, ?);";

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            for (Trip trip : trips) {
                ps.setInt(1, trip.getTripID());
                ps.setInt(2, trip.getCarID());
                ps.setDate(3, Date.valueOf(trip.getDate()));
                ps.setString(4, trip.getDeparture());
                ps.setString(5, trip.getDestination());
                ps.setInt(6, trip.getNum_seats_available());
                rowsInserted += ps.executeUpdate();
            }
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return rowsInserted;
    }

    //5.2 Task 2 Insert Bookings
    @Override
    public int insertBookings(Booking[] bookings) {
        int rowsInserted = 0;

        String query = "INSERT INTO Bookings " +
                "VALUES (?, ?, ?);";

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            for (Booking booking : bookings) {
                ps.setInt(1, booking.getTripID());
                ps.setInt(2, booking.getPIN());
                ps.setString(3, booking.getBooking_status());
                rowsInserted += ps.executeUpdate();
            }
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return rowsInserted;
    }


    //5.3 Task 3 Find all participants who are recorded as both drivers and passengers
    @Override
    public Participant[] getBothPassengersAndDrivers() {
        List<Participant> participants = new ArrayList<>();

        String query = "SELECT P1.PIN, P1.p_name, P1.age FROM Participants P1, Passengers P2, Drivers D " +
                "WHERE P1.PIN = P2.PIN AND P2.PIN = D.PIN " +
                "ORDER BY P1.PIN ASC;";

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                int PIN = rs.getInt("PIN");
                String p_name = rs.getString("p_name");
                int age = rs.getInt("age");

                // Create a Driver object with only PIN and rating
                Participant participant = new Participant(PIN, p_name, age);
                participants.add(participant);
            }
            rs.close();
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return participants.toArray(new Participant[0]);
    }


    //5.4 Task 4 Find the PINs, names, ages, and ratings of drivers who do not own any cars
    @Override
    public QueryResult.DriverPINNameAgeRating[] getDriversWithNoCars() {
        List<QueryResult.DriverPINNameAgeRating> drivers = new ArrayList<>();

        String query = "SELECT D.PIN, P.p_name, P.age, D.rating FROM Participants P, Drivers D " +
                "WHERE P.PIN = D.PIN AND D.PIN NOT IN (" +
                "SELECT C.PIN FROM Cars C) " +
                "ORDER BY D.PIN ASC;";

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                int PIN = rs.getInt("PIN");
                String p_name = rs.getString("p_name");
                int age = rs.getInt("age");
                double rating = rs.getDouble("rating");

                QueryResult.DriverPINNameAgeRating driver = new QueryResult.DriverPINNameAgeRating(PIN, p_name, age, rating);
                drivers.add(driver);
            }
            rs.close();
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return drivers.toArray(new QueryResult.DriverPINNameAgeRating[0]);
    }


    //5.5 Task 5 Delete Drivers who do not own any cars
    @Override
    public int deleteDriversWithNoCars() {
        int rowsDeleted = 0;

        String query = "DELETE FROM Drivers WHERE PIN = ?";

        QueryResult.DriverPINNameAgeRating[] driversWithNoCars = getDriversWithNoCars();

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            for (QueryResult.DriverPINNameAgeRating driver : driversWithNoCars) {
                ps.setInt(1, driver.getPIN());
                rowsDeleted += ps.executeUpdate();
            }
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return rowsDeleted;
    }


    //5.6 Task 6 Find all cars that are not taken part in any trips
    @Override
    public Car[] getCarsWithNoTrips() {
        List<Car> cars = new ArrayList<>();

        String query = "SELECT C.CarID, C.PIN, C.color, C.brand FROM Cars C " +
                "WHERE C.CarID NOT IN ( " +
                "SELECT T.CarID FROM Trips T)" +
                "ORDER BY C.CarID ASC;";


        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                int CarID = rs.getInt("CarID");
                int PIN = rs.getInt("PIN");
                String color = rs.getString("color");
                String brand = rs.getString("brand");

                Car car = new Car(CarID, PIN, color, brand);
                cars.add(car);
            }
            rs.close();
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return cars.toArray(new Car[0]);
    }


    //5.7 Task 7 Find all passengers who didn't book any trips
    @Override
    public Passenger[] getPassengersWithNoBooks() {
        List<Passenger> passengers = new ArrayList<>();

        String query = "SELECT P.PIN, P.membership_status FROM Passengers P " +
                "WHERE P.PIN NOT IN ( " +
                "SELECT B.PIN FROM Bookings B)" +
                "ORDER BY P.PIN ASC;";

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                int PIN = rs.getInt("PIN");
                String membership_status = rs.getString("membership_status");

                Passenger passenger = new Passenger(PIN, membership_status);
                passengers.add(passenger);
            }
            rs.close();
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return passengers.toArray(new Passenger[0]);
    }


    //5.8 Task 8 Find all trips that depart from the specified city to specified destination city on specific date
    @Override
    public Trip[] getTripsFromToCitiesOnSpecificDate(String departure, String destination, String date) {
        List<Trip> trips = new ArrayList<>();

        String query = "SELECT T.TripID, T.CarID, T.date, T.departure, T.destination, T.num_seats_available FROM Trips T " +
                "WHERE T.departure = ? AND T.destination = ? AND T.date = ? " +
                "ORDER BY T.TripID ASC;";

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            ps.setString(1, departure);
            ps.setString(2, destination);
            ps.setDate(3, Date.valueOf(date));
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                int tripID = rs.getInt("TripID");
                int carID = rs.getInt("CarID");
                int num_seats_available = rs.getInt("num_seats_available");

                Trip trip = new Trip(tripID, carID, date, departure, destination,num_seats_available);
                trips.add(trip);
            }
            rs.close();
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return trips.toArray(new Trip[0]);
    }


    //5.9 Task 9 Find the PINs, names, ages, and membership_status of passengers who have bookings on all trips destined at a particular city
    @Override
    public QueryResult.PassengerPINNameAgeMembershipStatus[] getPassengersWithBookingsToAllTripsForCity(String city) {
        List<QueryResult.PassengerPINNameAgeMembershipStatus> passengers = new ArrayList<>();

        String query = "SELECT P1.PIN, P1.p_name, P1.age, P2.membership_status FROM Participants P1, Passengers P2 " +
                "WHERE P1.PIN = P2.PIN AND NOT EXISTS (" +
                    "SELECT T.TripID FROM Trips T " +
                    "WHERE T.destination = ? AND NOT EXISTS (" +
                        "SELECT B.TripID FROM Bookings B " +
                        "WHERE B.TripID = T.TripID AND B.PIN = P1.PIN)) " +
                "ORDER BY P1.PIN ASC;";

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            ps.setString(1, city);
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                int PIN = rs.getInt("PIN");
                String p_name = rs.getString("p_name");
                int age = rs.getInt("age");
                String membership_status = rs.getString("membership_status");

                QueryResult.PassengerPINNameAgeMembershipStatus passenger = new QueryResult.PassengerPINNameAgeMembershipStatus(PIN, p_name, age, membership_status);
                passengers.add(passenger);
            }
            rs.close();
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return passengers.toArray(new QueryResult.PassengerPINNameAgeMembershipStatus[0]);
    }


    //5.10 Task 10 For a given driver PIN, find the CarIDs that the driver owns and were booked at most twice.
    @Override
    public Integer[] getDriverCarsWithAtMost2Bookings(int driverPIN) {
        List<Integer> carIds = new ArrayList<>();

        String query = "SELECT C.CarID " +
                "FROM Cars C, Trips T, Bookings B " +
                "WHERE C.CarID = T.CarID AND T.TripID = B.TripID AND C.PIN = ? " +
                "GROUP BY C.CarID " +
                "HAVING COUNT(B.TripID) <= 2 " +
                "ORDER BY C.CarID ASC;";

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            ps.setInt(1, driverPIN);
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                carIds.add(rs.getInt("CarID"));
            }
            rs.close();
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return carIds.toArray(new Integer[0]);
    }


    //5.11 Task 11 Find the average age of passengers with "Confirmed" bookings (i.e., booking_status is ”Confirmed”) on trips departing from a given city and within a specified date range
    @Override
    public Double getAvgAgeOfPassengersDepartFromCityBetweenTwoDates(String city, String start_date, String end_date) {
        Double averageAge = null;

        String query = "SELECT AVG(P2.age) " +
                "FROM Passengers P1, Participants P2, Trips T, Bookings B " +
                "WHERE P1.PIN = P2.PIN AND T.departure = ? AND T.date >= ? AND T.date <= ? AND B.TripID = T.TripID AND B.PIN = P1.PIN AND B.booking_status = 'Confirmed';";

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            ps.setString(1, city);
            ps.setDate(2, Date.valueOf(start_date));
            ps.setDate(3, Date.valueOf(end_date));
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                averageAge = rs.getDouble(1);
            }
            rs.close();
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return averageAge;
    }


    //5.12 Task 12 Find Passengers in a Given Trip.
    @Override
    public QueryResult.PassengerPINNameAgeMembershipStatus[] getPassengerInGivenTrip(int TripID) {
        List<QueryResult.PassengerPINNameAgeMembershipStatus> passengers = new ArrayList<>();

        String query = "SELECT P1.PIN, P1.p_name, P1.age, P2.membership_status FROM Participants P1, Passengers P2, Bookings B " +
                "WHERE P1.PIN = P2.PIN AND B.TripID = ? AND P1.PIN = B.PIN " +
                "ORDER BY P1.PIN ASC;";

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            ps.setInt(1, TripID);
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                int PIN = rs.getInt("PIN");
                String p_name = rs.getString("p_name");
                int age = rs.getInt("age");
                String membership_status = rs.getString("membership_status");

                QueryResult.PassengerPINNameAgeMembershipStatus passenger = new QueryResult.PassengerPINNameAgeMembershipStatus(PIN, p_name, age, membership_status);
                passengers.add(passenger);
            }
            rs.close();
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return passengers.toArray(new QueryResult.PassengerPINNameAgeMembershipStatus[0]);
    }


    //5.13 Task 13 Find Drivers’ Scores
    @Override
    public QueryResult.DriverScoreRatingNumberOfBookingsPIN[] getDriversScores() {
        List<QueryResult.DriverScoreRatingNumberOfBookingsPIN> drivers = new ArrayList<>();

        String query = "SELECT D.PIN, D.rating, COUNT(B.TripID) AS num_bookings FROM Drivers D, Bookings B, Trips T, Cars C " +
                "WHERE T.TripID = B.TripID AND C.PIN = D.PIN AND T.CarID = C.CarID " +
                "GROUP BY D.PIN, D.rating " +
                "HAVING COUNT(B.TripID) > 0" +
                "ORDER BY (D.rating * COUNT(B.TripID)) DESC, d.PIN ASC;";

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                int PIN = rs.getInt("PIN");
                Double rating = rs.getDouble("rating");
                int num_bookings = rs.getInt("num_bookings");
                Double driver_score = rating * num_bookings;
                QueryResult.DriverScoreRatingNumberOfBookingsPIN driver = new QueryResult.DriverScoreRatingNumberOfBookingsPIN(driver_score, rating, num_bookings, PIN);
                drivers.add(driver);
            }
            rs.close();
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return drivers.toArray(new QueryResult.DriverScoreRatingNumberOfBookingsPIN[0]);
    }


    //5.14 Task 14 Find average ratings of drivers who have trips destined to each city
    @Override
    public QueryResult.CityAndAverageDriverRating[] getDriversAverageRatingsToEachDestinatedCity() {
        List<QueryResult.CityAndAverageDriverRating> cityAndAverageDriverRatings = new ArrayList<>();

        String query = "SELECT AVG(D.rating) AS average_rating, T.destination FROM Drivers D, Cars C, Trips T " +
                "WHERE C.PIN = D.PIN AND T.CarID = C.CarID " +
                "GROUP BY T.destination " +
                "ORDER BY T.destination ASC;";

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                Double average_rating = rs.getDouble("average_rating");
                String destination = rs.getString("destination");
                QueryResult.CityAndAverageDriverRating cityAndAverageDriverRating = new QueryResult.CityAndAverageDriverRating(destination, average_rating);
                cityAndAverageDriverRatings.add(cityAndAverageDriverRating);
            }
            rs.close();
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return cityAndAverageDriverRatings.toArray(new QueryResult.CityAndAverageDriverRating[0]);
    }


    //5.15 Task 15 Find total number of bookings of passengers for each membership status
    @Override
    public QueryResult.MembershipStatusAndTotalBookings[] getTotalBookingsEachMembershipStatus() {
        List<QueryResult.MembershipStatusAndTotalBookings> membershipStatusAndTotalBookings = new ArrayList<>();

        String query = "SELECT COUNT(B.TripID) AS total_number_of_bookings, P.membership_status FROM Passengers P, Bookings B " +
                "WHERE P.PIN = B.PIN " +
                "GROUP BY P.membership_status " +
                "ORDER BY P.membership_status ASC;";

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                String membership_status = rs.getString("membership_status");
                int total_number_of_bookings = rs.getInt("total_number_of_bookings");
                QueryResult.MembershipStatusAndTotalBookings membershipStatusAndTotalBooking = new QueryResult.MembershipStatusAndTotalBookings(membership_status, total_number_of_bookings);
                membershipStatusAndTotalBookings.add(membershipStatusAndTotalBooking);
            }
            rs.close();
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return membershipStatusAndTotalBookings.toArray(new QueryResult.MembershipStatusAndTotalBookings[0]);
    }


    //5.16 Task 16 For the drivers' ratings, if rating is smaller than 2.0 or equal to 2.0, update the rating by adding 0.5.
    @Override
    public int updateDriverRatings() {
        int rowsUpdated = 0;

        String query = "UPDATE Drivers SET rating = rating + 0.5 WHERE rating <= 2.0";

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            rowsUpdated = ps.executeUpdate();
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return rowsUpdated;
    }


    //6.1 (Optional) Task 18 Find trips departing from the given city
    @Override
    public Trip[] getTripsFromCity(String city) {
        List<Trip> trips = new ArrayList<>();

        String query = "SELECT T.TripID, T.CarID, T.date, T.destination, T.num_seats_available FROM Trips T " +
                "WHERE T.departure = ? " +
                "ORDER BY T.TripID ASC;";

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            ps.setString(1, city);
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                int tripID = rs.getInt("TripID");
                int carID = rs.getInt("CarID");
                String date = rs.getDate("date").toString();
                String destination = rs.getString("destination");
                int num_seats_available = rs.getInt("num_seats_available");
                Trip trip = new Trip(tripID, carID, date, city, destination, num_seats_available);
                trips.add(trip);
            }
            rs.close();
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return trips.toArray(new Trip[0]);
    }


    //6.2 (Optional) Task 19 Find all trips that have never been booked
    @Override
    public Trip[] getTripsWithNoBooks() {
        List<Trip> trips = new ArrayList<>();

        String query = "SELECT T.TripID, T.CarID, T.date, T.departure, T.destination, T.num_seats_available FROM Trips T " +
                "WHERE NOT EXISTS(" +
                    "SELECT B.TripID FROM Bookings B WHERE T.TripID = B.TripID) " +
                "ORDER BY T.TripID ASC;";

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                int tripID = rs.getInt("TripID");
                int carID = rs.getInt("CarID");
                String date = rs.getDate("date").toString();
                String departure = rs.getString("departure");
                String destination = rs.getString("destination");
                int num_seats_available = rs.getInt("num_seats_available");
                Trip trip = new Trip(tripID, carID, date, departure, destination, num_seats_available);
                trips.add(trip);
            }
            rs.close();
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return trips.toArray(new Trip[0]);
    }


    //6.3 (Optional) Task 20 For each driver, find the trip(s) with the highest number of bookings
    @Override
    public QueryResult.DriverPINandTripIDandNumberOfBookings[] getTheMostBookedTripsPerDriver() {
         return new QueryResult.DriverPINandTripIDandNumberOfBookings[0];
    }


    //6.4 (Optional) Task 21 Find Full Cars
    @Override
    public QueryResult.FullCars[] getFullCars() {

        /*****************************************************/
        /*****************************************************/
        /*****************  TODO  (Optional)   ***************/
        /*****************************************************/
        /*****************************************************/

        return new QueryResult.FullCars[0];
    }

}
