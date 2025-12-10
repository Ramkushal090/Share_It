package com.example.demo1;

import javafx.animation.TranslateTransition;
import javafx.scene.control.Button;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.layout.Priority;
import javafx.scene.control.Label;
import javafx.util.Duration;
import javafx.geometry.Pos;
import javafx.geometry.Insets;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.stage.Stage;

public class ShareItApp extends Application {
    private ListView<String> listingView;
    private ListView<String> requestView;
    private HBox contentContainer;
    private VBox detailsPanel;
    private boolean isPanelOpen = false;
    private Stage formStage = null;
    private VBox mainContainer;
    private Stage notificationStage;
    private Stage profileStage;


    // Native method declarations for JNI
    private native void initializeNative();
    private native String[] getListings();
    private native String[] getRequests();
    private native String getItemDetails(boolean isListing, int index);
    private native boolean saveNewListing(String name, String condition, String price,
                                          String quantity, String owner, String fromDate,
                                          String toDate);
    private native boolean saveNewRequest(String name, String condition, String price,
                                          String quantity, String owner, String fromDate,
                                          String toDate);
    private native String[] getNotifications();
    private native boolean sendNotificationResponse(String notificationId, boolean isAccepted);
    private native void requestListing(boolean isListing, int index);
    private native String[] getMyListings();
    private native String[] getMyRequests();
    private native boolean saveListingChanges(String listingId, String name, String condition, String price, String quantity, String owner, String fromDate, String toDate);
    private native boolean deleteListing(String listingId);
    private native int getCoins();


    // Load native library
    static {
        System.loadLibrary("shareit_native");
    }

    @Override
    public void start(Stage primaryStage) {
        // Initialize native backend
        initializeNative();

        primaryStage.setTitle("ShareIt - Community Sharing Platform");
        // Create main container with dark theme
        mainContainer = new VBox(20);
        mainContainer.setStyle(
                "-fx-background-color: #1a1a1a;" +
                        "-fx-padding: 30;"
        );

        // Header section
        HBox header = createHeader();
        // Main content section with lists and details panel
        contentContainer = new HBox(30);
        contentContainer.setAlignment(Pos.CENTER);
        contentContainer.setTranslateX(175);

        // Create lists section
        VBox listsSection = createListsSection();

        // Create details panel (initially hidden)
        detailsPanel = createDetailsPanel();
        detailsPanel.setTranslateX(800); // Start off-screen

        contentContainer.getChildren().addAll(listsSection, detailsPanel);

        // Action buttons section at bottom
        HBox buttonSection = createActionButtons();

        // Add all sections to main container
        mainContainer.getChildren().addAll(header, contentContainer, buttonSection);
        VBox.setVgrow(contentContainer, Priority.ALWAYS);

        Scene scene = new Scene(mainContainer, 1200, 800);
        scene.getStylesheets().add(getClass().getResource("dark-theme.css").toExternalForm());
        primaryStage.setScene(scene);
        primaryStage.show();

        // Load initial data
        loadData();
    }

    private void loadData() {
        // Get data from native backend
        String[] listings = getListings();
        String[] requests = getRequests();

        // Clear and populate list views
        listingView.getItems().clear();
        requestView.getItems().clear();

        if (listings != null) {
            listingView.getItems().addAll(listings);
        }

        if (requests != null) {
            requestView.getItems().addAll(requests);
        }
    }

    private void loadListings(ListView<String> listingsView) {
        try {
            // Fetch full listings data
            String[] listings = getMyListings(); // Example: "Name|Condition|Price|..."

            // Display only names in the ListView
            listingsView.getItems().clear();
            for (String listing : listings) {
                String name = extractNameFromListing(listing); // Extract the name
                listingsView.getItems().add(name);
            }

            // Store full data for later use
            listingsView.setUserData(listings);
        } catch (Exception e) {
            e.printStackTrace();
            listingsView.getItems().add("Error loading listings.");
        }
    }

    private void loadRequests(ListView<String> requestsView) {
        try {
            // Fetch full requests data
            String[] requests = getMyRequests(); // Example: "Name|Condition|Price|..."

            // Display only names in the ListView
            requestsView.getItems().clear();
            for (String request : requests) {
                String name = extractNameFromListing(request); // Extract the name
                requestsView.getItems().add(name);
            }

            // Store full data for later use
            requestsView.setUserData(requests);
        } catch (Exception e) {
            e.printStackTrace();
            requestsView.getItems().add("Error loading requests.");
        }
    }

    private String extractNameFromListing(String listingData) {
        if (listingData != null && listingData.contains("|")) {
            return listingData.split("\\|")[0].trim(); // Return the first field as the name
        }
        return listingData; // Fallback if the format is unexpected
    }

    private HBox createHeader() {
        Label titleLabel = new Label("ShareIt");
        titleLabel.setStyle(
                "-fx-font-size: 36px; " +
                        "-fx-font-weight: bold; " +
                        "-fx-text-fill: #ffffff;"
        );


        Button notificationButton = new Button("🔔");
        notificationButton.setStyle(
                "-fx-background-color: transparent; " +
                        "-fx-text-fill: white; " +
                        "-fx-font-size: 18px; " +
                        "-fx-cursor: hand;"
        );
        notificationButton.setOnAction(e -> showNotificationWindow()); // Open the popup window
        Button profileButton = new Button("My Profile");
        profileButton.setStyle(
                "-fx-background-color: transparent; " +
                        "-fx-text-fill: white; " +
                        "-fx-font-size: 18px; " +
                        "-fx-cursor: hand;"
        );
        profileButton.setOnAction(e -> showProfileWindow());
        HBox header = new HBox(400,profileButton, titleLabel, notificationButton);
        header.setAlignment(Pos.TOP_CENTER);
        return header;
    }

    private void showNotificationWindow() {
        if (notificationStage == null) {
            notificationStage = new Stage();
            notificationStage.initOwner(mainContainer.getScene().getWindow()); // Link to the main window
            notificationStage.setTitle("Notifications");

            VBox notificationBox = new VBox(15);
            notificationBox.setStyle(
                    "-fx-background-color: #2d2d2d; " +
                            "-fx-padding: 20; " +
                            "-fx-background-radius: 10; " +
                            "-fx-border-radius: 10; " +
                            "-fx-border-color: #3d3d3d;"
            );

            Label titleLabel = new Label("Notifications");
            titleLabel.setStyle("-fx-font-size: 24px; -fx-font-weight: bold; -fx-text-fill: white;");

            ListView<String> notificationList = new ListView<>();
            notificationList.setStyle(
                    "-fx-background-color: #2d2d2d; " +
                            "-fx-control-inner-background: #2d2d2d; " +
                            "-fx-border-color: #3d3d3d; " +
                            "-fx-text-fill: white;"
            );

            Button refreshButton = new Button("Refresh");
            refreshButton.setStyle(
                    "-fx-background-color: #1a365d; " +
                            "-fx-text-fill: white; " +
                            "-fx-font-size: 14px; " +
                            "-fx-cursor: hand;"
            );
            refreshButton.setOnAction(e -> loadNotifications(notificationList)); // Refresh notifications

            Button closeButton = new Button("Close");
            closeButton.setStyle(
                    "-fx-background-color: #1e4620; " +
                            "-fx-text-fill: white; " +
                            "-fx-font-size: 14px; " +
                            "-fx-cursor: hand;"
            );
            closeButton.setOnAction(e -> notificationStage.close());

            HBox buttonBox = new HBox(10, refreshButton, closeButton);
            buttonBox.setAlignment(Pos.CENTER);

            notificationBox.getChildren().addAll(titleLabel, notificationList, buttonBox);
            notificationBox.setAlignment(Pos.CENTER);

            Scene notificationScene = new Scene(notificationBox, 400, 300);
            notificationStage.setScene(notificationScene);
        }

        // Load notifications before showing the popup
        if (notificationStage.isShowing() == false) {
            ListView<String> notificationList = (ListView<String>) notificationStage.getScene()
                    .lookup(".list-view");
            loadNotifications(notificationList);
        }

        // Show the popup window
        notificationStage.show();
    }

    private void loadNotifications(ListView<String> notificationList) {
        try {
            // Call native method to fetch notifications
            String[] notifications = getNotifications();

            // Clear the existing list and populate new notifications
            notificationList.getItems().clear();
            if (notifications != null && notifications.length > 0) {
                notificationList.getItems().addAll(notifications);
            } else {
                notificationList.getItems().add("No new notifications.");
            }

            // Handle click on a notification
            notificationList.setOnMouseClicked(event -> {
                if (event.getClickCount() == 2) { // Double-click to interact
                    int selectedIndex = notificationList.getSelectionModel().getSelectedIndex();
                    String selectedNotification = notificationList.getSelectionModel().getSelectedItem();

                    if (selectedIndex >= 0 && selectedNotification != null && !selectedNotification.equals("No new notifications.")) {
                        showNotificationActionDialog(selectedIndex, selectedNotification);
                    }
                }
            });
        } catch (Exception e) {
            notificationList.getItems().clear();
            notificationList.getItems().add("Error fetching notifications.");
            e.printStackTrace();
        }
    }

    private void showNotificationActionDialog(int notificationIndex, String notification) {
        Stage dialog = new Stage();
        dialog.initOwner(notificationStage);
        dialog.setTitle("Notification Action");

        VBox dialogBox = new VBox(20);
        dialogBox.setStyle("-fx-background-color: #2d2d2d; -fx-padding: 20; -fx-background-radius: 10;");

        Label messageLabel = new Label("Notification: " + notification);
        messageLabel.setStyle("-fx-text-fill: white; -fx-font-size: 16px;");

        Button acceptButton = new Button("Accept");
        acceptButton.setStyle(
                "-fx-background-color: #1e4620; -fx-text-fill: white; -fx-font-size: 14px; -fx-cursor: hand;"
        );
        acceptButton.setOnAction(e -> {
            handleNotificationResponse(notificationIndex, true);
            dialog.close();
        });

        Button declineButton = new Button("Decline");
        declineButton.setStyle(
                "-fx-background-color: #7d1e1e; -fx-text-fill: white; -fx-font-size: 14px; -fx-cursor: hand;"
        );
        declineButton.setOnAction(e -> {
            handleNotificationResponse(notificationIndex, false);
            dialog.close();
        });

        HBox buttonBox = new HBox(10, acceptButton, declineButton);
        buttonBox.setAlignment(Pos.CENTER);

        dialogBox.getChildren().addAll(messageLabel, buttonBox);
        dialogBox.setAlignment(Pos.CENTER);

        Scene dialogScene = new Scene(dialogBox, 300, 150);
        dialog.setScene(dialogScene);
        dialog.show();
    }

    private void handleNotificationResponse(int notificationIndex, boolean isAccepted) {
        // Convert index to string for the native method
        String notificationId = String.valueOf(notificationIndex);

        // Call the JNI method to send the response
        boolean success = sendNotificationResponse(notificationId, isAccepted);

        if (success) {
            showSuccessAlert("Response Sent", isAccepted ? "You accepted the notification." : "You declined the notification.");
        } else {
            showAlert("Error", "Failed to send the response.");
        }

        // Optionally refresh the notifications list
        ListView<String> notificationList = (ListView<String>) notificationStage.getScene().lookup(".list-view");
        if (notificationList != null) {
            loadNotifications(notificationList);
        }
    }

    private void showProfileWindow() {
        if (profileStage == null) {
            profileStage = new Stage();
            profileStage.setTitle("My Profile");

            VBox profileBox = new VBox(20);
            profileBox.setStyle("-fx-background-color: #2d2d2d; -fx-padding: 20; -fx-background-radius: 10;");

            Label titleLabel = new Label("My Profile");
            titleLabel.setStyle("-fx-font-size: 24px; -fx-font-weight: bold; -fx-text-fill: white;");

            // Coins indicator at the top-right
            Label coinsLabel = new Label("Coins: 0");
            coinsLabel.setStyle("-fx-font-size: 18px; -fx-text-fill: gold;");
            int coins = getCoins(); // Call native function to get coins
            coinsLabel.setText("Coins: " + coins);

            ListView<String> listingsView = new ListView<>();
            ListView<String> requestsView = new ListView<>();
            listingsView.setStyle(
                    "-fx-background-color: #2d2d2d;" + // Dark background
                            "-fx-control-inner-background: #2d2d2d;" + // Inner background
                            "-fx-text-fill: white;" // Text color
            );
            requestsView.setStyle(
                    "-fx-background-color: #2d2d2d;" +
                            "-fx-control-inner-background: #2d2d2d;" +
                            "-fx-text-fill: white;"
            );
            VBox listingsBox = new VBox(
                    new Label("Listings") {{
                        setStyle("-fx-font-size: 18px; -fx-text-fill: white;");
                    }},
                    listingsView
            );
            VBox requestsBox = new VBox(
                    new Label("Requests") {{
                        setStyle("-fx-font-size: 18px; -fx-text-fill: white;");
                    }},
                    requestsView
            );

            listingsBox.setSpacing(10);
            requestsBox.setSpacing(10);


            // Fetch and populate listings and requests
            loadListings(listingsView);
            loadRequests(requestsView);
            listingsView.setOnMouseClicked(event -> {
                if (event.getClickCount() == 2) { // Double-click to interact
                    int selectedIndex = listingsView.getSelectionModel().getSelectedIndex();
                    if (selectedIndex >= 0) {
                        String[] listings = (String[]) listingsView.getUserData(); // Full data
                        String selectedListing = listings[selectedIndex];
                        showEditListingWindow(selectedIndex, selectedListing, true); // Open edit window
                    }
                }
            });


            requestsView.setOnMouseClicked(event -> {
                if (event.getClickCount() == 2) { // Double-click to interact
                    int selectedIndex = requestsView.getSelectionModel().getSelectedIndex();
                    if (selectedIndex >= 0) {
                        String[] requests = (String[]) requestsView.getUserData(); // Full data
                        String selectedRequest = requests[selectedIndex];
                        showEditListingWindow(selectedIndex, selectedRequest, false); // Open edit window
                    }
                }
            });

            HBox contentBox = new HBox(20, listingsBox, requestsBox);
            contentBox.setAlignment(Pos.CENTER);
            HBox title = new HBox(250, titleLabel, coinsLabel);
            profileBox.getChildren().addAll(title, contentBox);
            Scene profileScene = new Scene(profileBox, 600, 400);
            profileStage.setScene(profileScene);
        }

        profileStage.show();
    }

    private void showEditListingWindow(int index, String listingData, boolean isListing) {
        Stage editStage = new Stage();
        editStage.setTitle(isListing ? "Edit Listing" : "Edit Request");

        VBox editBox = new VBox(15);
        editBox.setStyle("-fx-background-color: #2d2d2d; -fx-padding: 20; -fx-background-radius: 10;");

        Label titleLabel = new Label(isListing ? "Edit Listing" : "Edit Request");
        titleLabel.setStyle("-fx-font-size: 24px; -fx-font-weight: bold; -fx-text-fill: white;");

        // Parse the listing data
        String[] fields = listingData.split("\\|"); // Assuming data is in a pipe-separated format
        TextField nameField = new TextField(fields[0]);
        TextField conditionField = new TextField(fields[1]);
        TextField priceField = new TextField(fields[2]);
        TextField quantityField = new TextField(fields[3]);
        TextField ownerField = new TextField(fields[4]);
        TextField fromDateField = new TextField(fields[5]);
        TextField toDateField = new TextField(fields[6]);

        VBox formBox = new VBox(
                new Label("Name:"), nameField,
                new Label("Condition:"), conditionField,
                new Label("Price:"), priceField,
                new Label("Quantity:"), quantityField,
                new Label("Owner:"), ownerField,
                new Label("From Date:"), fromDateField,
                new Label("To Date:"), toDateField
        );

        Button saveButton = new Button("Save Changes");
        saveButton.setOnAction(e -> {
            boolean success = saveListingChanges(
                    String.valueOf(index), // Use index as ID
                    nameField.getText(), conditionField.getText(),
                    priceField.getText(), quantityField.getText(),
                    ownerField.getText(), fromDateField.getText(),
                    toDateField.getText()
            );
            if (success) {
                showSuccessAlert("Success", "Changes saved successfully.");
                editStage.close();
            } else {
                showAlert("Error", "Failed to save changes.");
            }
        });

        Button deleteButton = new Button("Delete");
        deleteButton.setStyle("-fx-background-color: #7d1e1e; -fx-text-fill: white;");
        deleteButton.setOnAction(e -> {
            boolean success = deleteListing(String.valueOf(index)); // Use index as ID
            if (success) {
                showSuccessAlert("Success", isListing ? "Listing deleted." : "Request deleted.");
                editStage.close();
            } else {
                showAlert("Error", "Failed to delete.");
            }
        });

        HBox buttonBox = new HBox(10, saveButton, deleteButton);
        buttonBox.setAlignment(Pos.CENTER);

        editBox.getChildren().addAll(titleLabel, formBox, buttonBox);
        Scene editScene = new Scene(editBox, 400, 500);
        editStage.setScene(editScene);
        editStage.show();
    }

    private VBox createListsSection() {
        listingView = createStyledListView();
        requestView = createStyledListView();

        listingView.setOnMouseClicked(event -> handleItemClick(listingView, true));
        requestView.setOnMouseClicked(event -> handleItemClick(requestView, false));

//        loadData();

        Label listingsLabel = createSectionLabel("Available Listings");
        Label requestsLabel = createSectionLabel("Current Requests");

        VBox listingSection = createSection(listingsLabel, listingView);
        VBox requestSection = createSection(requestsLabel, requestView);

        HBox listsContainer = new HBox(50, listingSection, requestSection);
        listsContainer.setAlignment(Pos.CENTER);

        VBox listSection = new VBox(20, listsContainer);
        listSection.setAlignment(Pos.CENTER); // Center the lists initially
        return listSection;
    }

    private VBox createSection(Label label, ListView<String> listView) {
        VBox section = new VBox(10);
        section.setAlignment(Pos.CENTER);
        section.setPadding(new Insets(10));
        section.setStyle(
                "-fx-background-color: #2d2d2d; " +
                        "-fx-background-radius: 10; " +
                        "-fx-border-radius: 10; " +
                        "-fx-border-color: #3d3d3d;"
        );

        section.getChildren().addAll(label, listView);
        return section;
    }

    private Label createSectionLabel(String text) {
        Label label = new Label(text);
        label.setStyle(
                "-fx-font-size: 18px; " +
                        "-fx-font-weight: bold; " +
                        "-fx-text-fill: #ffffff; " +
                        "-fx-padding: 5 0 5 0;"
        );
        return label;
    }

    private ListView<String> createStyledListView() {
        ListView<String> listView = new ListView<>();
        listView.setStyle(
                "-fx-background-color: #2d2d2d; " +
                        "-fx-control-inner-background: #2d2d2d; " +
                        "-fx-background-radius: 5; " +
                        "-fx-border-radius: 5; " +
                        "-fx-border-color: #3d3d3d; " +
                        "-fx-text-fill: white;"
        );
        listView.setPrefWidth(350);
        listView.setPrefHeight(500);
        return listView;
    }

    private VBox createDetailsPanel() {
        VBox panel = new VBox(15);
        panel.setStyle(
                "-fx-background-color: #2d2d2d; " +
                        "-fx-padding: 20; " +
                        "-fx-background-radius: 10; " +
                        "-fx-border-radius: 10; " +
                        "-fx-border-color: #3d3d3d; " +
                        "-fx-min-width: 300;"
        );
        panel.setPrefWidth(350);

        Button closeButton = new Button("×");
        closeButton.setStyle(
                "-fx-background-color: transparent; " +
                        "-fx-text-fill: white; " +
                        "-fx-font-size: 20px; " +
                        "-fx-cursor: hand;"
        );
        closeButton.setOnAction(e -> closeDetailsPanel());

        HBox header = new HBox();
        header.setAlignment(Pos.TOP_RIGHT);
        header.getChildren().add(closeButton);

        panel.getChildren().add(header);
        return panel;
    }

    private void handleItemClick(ListView<String> listView, boolean isListing) {
        int selectedIndex = listView.getSelectionModel().getSelectedIndex();
        if (selectedIndex >= 0) {
            // Get item details from native backend
            String itemDetails = getItemDetails(isListing, selectedIndex);
            showDetailsPanel(itemDetails, isListing, selectedIndex);
        }
    }

    private void showDetailsPanel(String itemDetails, boolean isListing, int selectedIndex) {
        if (contentContainer.getChildren().size() > 2) {
            contentContainer.getChildren().remove(2);
        }

        isPanelOpen = true;

        // Clear previous content
        detailsPanel.getChildren().subList(1, detailsPanel.getChildren().size()).clear();

        // Parse itemDetails and create UI elements
        // Implementation note: itemDetails should be formatted in a way that can be easily parsed
        // For example: name|condition|price|quantity|owner|fromDate|toDate
        String[] details = itemDetails.split("\\|");
        String[] labels = {"Name", "Condition", "Price", "Quantity", "Owner", "From Date", "To Date"};

        for (int i = 0; i < details.length; i++) {
            VBox fieldBox = new VBox(5);
            Label label = new Label(labels[i]);
            label.setStyle("-fx-text-fill: #bebebe;");

            Label valueLabel = new Label(details[i]);
            valueLabel.setStyle("-fx-text-fill: white; -fx-font-size: 16px;");

            fieldBox.getChildren().addAll(label, valueLabel);
            detailsPanel.getChildren().add(fieldBox);
        }

        // Animation code remains the same
        if (contentContainer.getTranslateX() != 0) {
            contentContainer.setTranslateX(0);
            detailsPanel.setTranslateX(800);
        }

        Button requestButton = createStyledButton("Request/Fulfil", "#1e4620");
        requestButton.setMaxWidth(Double.MAX_VALUE);

        // Placeholder for request button action
        requestButton.setOnAction(e -> {
            System.out.println("Request button clicked!");
            // Add actual implementation here if needed
            requestListing(isListing, selectedIndex);
        });

        VBox footer = new VBox(10); // Adjust spacing as needed
        footer.setAlignment(Pos.BASELINE_CENTER);
        footer.getChildren().add(requestButton);
        detailsPanel.getChildren().add(footer);

        TranslateTransition slideList = new TranslateTransition(Duration.seconds(0.3), contentContainer);
        slideList.setToX(-200);

        TranslateTransition slidePanel = new TranslateTransition(Duration.seconds(0.3), detailsPanel);
        slidePanel.setToX(0);

        slideList.play();
        slidePanel.play();
    }

    private void closeDetailsPanel() {
        isPanelOpen = false;

        // Remove any existing form panel
        if (contentContainer.getChildren().size() > 2) {
            contentContainer.getChildren().remove(2);
        }

        // Animate lists back to center
        TranslateTransition slideList = new TranslateTransition(Duration.seconds(0.3), contentContainer);
        slideList.setToX(175);

        // Animate panel out
        TranslateTransition slidePanel = new TranslateTransition(Duration.seconds(0.3), detailsPanel);
        slidePanel.setToX(800);

        slideList.play();
        slidePanel.play();
    }

    private HBox createActionButtons() {
        Button createListingButton = createStyledButton("Create Listing", "#1e4620");
        Button createRequestButton = createStyledButton("Create Request", "#1a365d");

        createListingButton.setOnAction(e -> openCreateForm("Listing"));
        createRequestButton.setOnAction(e -> openCreateForm("Request"));

        HBox buttonBox = new HBox(20, createListingButton, createRequestButton);
        buttonBox.setAlignment(Pos.CENTER);
        buttonBox.setStyle("-fx-padding: 20 0 0 0;");
        return buttonBox;
    }

    private Button createStyledButton(String text, String color) {
        Button button = new Button(text);
        button.setStyle(
                "-fx-background-color: " + color + "; " +
                        "-fx-text-fill: white; " +
                        "-fx-font-size: 14px; " +
                        "-fx-padding: 10 20 10 20; " +
                        "-fx-background-radius: 5; " +
                        "-fx-cursor: hand;"
        );
        return button;
    }

    private void openCreateForm(String type) {
        if (formStage != null) {
            formStage.close();
        }

        // Create styled form fields
        TextField nameField = createStyledTextField("Enter name");
        TextField conditionField = createStyledTextField("Enter condition: excellent/good/fair/poor");
        TextField priceField = createStyledTextField("Enter price");
        TextField quantityField = createStyledTextField("Enter quantity");
        TextField ownerField = createStyledTextField("Enter owner name");
        TextField fromDateField = createStyledTextField("DD-MM-YYYY");
        TextField toDateField = createStyledTextField("DD-MM-YYYY");

        // Create form panel
        VBox formPanel = new VBox(15);
        formPanel.setStyle(
                "-fx-background-color: #2d2d2d; " +
                        "-fx-padding: 20; " +
                        "-fx-background-radius: 10; " +
                        "-fx-border-radius: 10; " +
                        "-fx-border-color: #3d3d3d; " +
                        "-fx-min-width: 300;"
        );
        formPanel.setPrefWidth(350);
        formPanel.setTranslateX(800); // Start off-screen

        // Create close button
        Button closeButton = new Button("×");
        closeButton.setStyle(
                "-fx-background-color: transparent; " +
                        "-fx-text-fill: white; " +
                        "-fx-font-size: 20px; " +
                        "-fx-cursor: hand;"
        );
        closeButton.setOnAction(e -> closeCreateForm());

        Label titleLabel = new Label("New " + type);
        titleLabel.setStyle(
                "-fx-font-size: 24px; " +
                        "-fx-font-weight: bold; " +
                        "-fx-text-fill: white;"
        );

        // Header with title and close button
        HBox header = new HBox();
        header.setAlignment(Pos.CENTER_LEFT);
        header.setSpacing(50);
        header.getChildren().addAll(titleLabel, closeButton);
        HBox.setHgrow(titleLabel, Priority.ALWAYS);

        // Add form fields with labels
        formPanel.getChildren().addAll(
                header,
                createFormField("Item Name", nameField),
                createFormField("Condition", conditionField),
                createFormField("Price", priceField),
                createFormField("Quantity", quantityField),
                createFormField("Owner", ownerField),
                createFormField("From Date", fromDateField),
                createFormField("To Date", toDateField)
        );

        // Create submit button
        Button submitButton = createStyledButton("Submit", "#1e4620");
        submitButton.setMaxWidth(Double.MAX_VALUE);
        submitButton.setOnAction(e -> {
            if (validateForm(nameField, priceField, quantityField)) {
                saveFormData(type, nameField, conditionField, priceField,
                        quantityField, ownerField, fromDateField, toDateField);
                closeCreateForm();
            }
        });

        formPanel.getChildren().add(submitButton);

        // Add form panel to the main content container
        contentContainer.getChildren().add(formPanel);

        // Animate lists to the left and form panel in
        if (!isPanelOpen) {  // Only animate lists if no panel is open
            TranslateTransition slideList = new TranslateTransition(Duration.seconds(0.3), contentContainer);
            slideList.setToX(-200);
            slideList.play();
        }

        TranslateTransition slideForm = new TranslateTransition(Duration.seconds(0.3), formPanel);
        slideForm.setToX(0);
        slideForm.play();

        isPanelOpen = true;
    }

    private void closeCreateForm() {
        VBox formPanel = (VBox) contentContainer.getChildren().get(contentContainer.getChildren().size() - 1);

        // Animate lists back to center
        TranslateTransition slideList = new TranslateTransition(Duration.seconds(0.3), contentContainer);
        slideList.setToX(175);

        // Animate form out
        TranslateTransition slideForm = new TranslateTransition(Duration.seconds(0.3), formPanel);
        slideForm.setToX(800);

        slideForm.setOnFinished(e -> {
            contentContainer.getChildren().remove(formPanel);
            isPanelOpen = false;
        });

        slideList.play();
        slideForm.play();
    }

    private TextField createStyledTextField(String prompt) {
        TextField field = new TextField();
        field.setPromptText(prompt);
        field.setStyle(
                "-fx-background-color: #2d2d2d; " +
                        "-fx-text-fill: white; " +
                        "-fx-prompt-text-fill: #808080; " +
                        "-fx-border-color: #3d3d3d; " +
                        "-fx-border-radius: 5; " +
                        "-fx-background-radius: 5; " +
                        "-fx-padding: 8;"
        );
        return field;
    }

    private void saveFormData(String type, TextField nameField, TextField conditionField,
                              TextField priceField, TextField quantityField, TextField ownerField,
                              TextField fromDateField, TextField toDateField) {
        boolean success;
        if (type.equals("Listing")) {
            success = saveNewListing(
                    nameField.getText(), conditionField.getText(),
                    priceField.getText(), quantityField.getText(),
                    ownerField.getText(), fromDateField.getText(),
                    toDateField.getText()
            );
        } else {
            success = saveNewRequest(
                    nameField.getText(), conditionField.getText(),
                    priceField.getText(), quantityField.getText(),
                    ownerField.getText(), fromDateField.getText(),
                    toDateField.getText()
            );
        }

        if (success) {
            showSuccessAlert("Success", type + " created successfully!");
            loadData(); // Reload the lists
        } else {
            showAlert("Error", "Failed to create " + type.toLowerCase());
        }
    }

    private VBox createFormField(String labelText, TextField field) {
        Label label = new Label(labelText);
        label.setStyle("-fx-font-weight: bold; -fx-text-fill: #bebebe;");
        VBox container = new VBox(5, label, field);
        return container;
    }

    private boolean validateForm(TextField nameField, TextField priceField, TextField quantityField) {
        if (nameField.getText().trim().isEmpty() ||
                priceField.getText().trim().isEmpty() ||
                quantityField.getText().trim().isEmpty()) {
            showAlert("Validation Error", "Please fill in all required fields.");
            return false;
        }
        try {
            Integer.parseInt(priceField.getText());
            Integer.parseInt(quantityField.getText());
        } catch (NumberFormatException e) {
            showAlert("Validation Error", "Price and Quantity must be valid numbers.");
            return false;
        }
        return true;
    }

    private void showAlert(String title, String content) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle(title);
        alert.setHeaderText(null);
        alert.setContentText(content);
        alert.getDialogPane().setStyle(
                "-fx-background-color: #c5c3c3;" +
                        "-fx-text-fill: white;"
        );
        // Style the dialog buttons
        alert.getDialogPane().lookupButton(ButtonType.OK)
                .setStyle("-fx-background-color: #1e4620; -fx-text-fill: white;");
        alert.showAndWait();
    }

    private void showSuccessAlert(String title, String content) {
        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setTitle(title);
        alert.setHeaderText(null);
        alert.setContentText(content);
        alert.getDialogPane().setStyle(
                "-fx-background-color: #c5c3c3;" +
                        "-fx-text-fill: #ffffff;"
        );
        // Style the dialog buttons
        alert.getDialogPane().lookupButton(ButtonType.OK)
                .setStyle("-fx-background-color: #1e4620; -fx-text-fill: white;");
        alert.showAndWait();
    }
}