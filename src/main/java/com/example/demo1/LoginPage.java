package com.example.demo1;

import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.*;
import javafx.stage.Stage;
import javafx.scene.image.ImageView;
import javafx.scene.image.Image;

public class LoginPage extends Application {
    private Stage primaryStage;
    private TextField usernameField;
    private PasswordField passwordField;

    // Native method for user validation
    private native boolean validateUser(String username, String password);

    // Load native library
    static {
        System.loadLibrary("shareit_native");
    }

    @Override
    public void start(Stage primaryStage) {
        this.primaryStage = primaryStage;
        primaryStage.setTitle("ShareIt - Login");

        // Create main container
        VBox mainContainer = new VBox(30);
        mainContainer.setStyle(
                "-fx-background-color: #1a1a1a;" +
                        "-fx-padding: 40;"
        );
        mainContainer.setAlignment(Pos.CENTER);

        // Add logo/header section
        VBox headerSection = createHeaderSection();

        // Create login form
        VBox loginForm = createLoginForm();

        // Add sections to main container
        mainContainer.getChildren().addAll(headerSection, loginForm);

        Scene scene = new Scene(mainContainer, 400, 600);
        scene.getStylesheets().add(getClass().getResource("dark-theme.css").toExternalForm());

        primaryStage.setScene(scene);
        primaryStage.show();
    }

    private VBox createHeaderSection() {
        Label titleLabel = new Label("ShareIt");
        titleLabel.setStyle(
                "-fx-font-size: 42px; " +
                        "-fx-font-weight: bold; " +
                        "-fx-text-fill: #ffffff;"
        );

        Label subtitleLabel = new Label("Your Community Sharing Platform");
        subtitleLabel.setStyle(
                "-fx-font-size: 16px; " +
                        "-fx-text-fill: #bebebe;"
        );

        VBox header = new VBox(10, titleLabel, subtitleLabel);
        header.setAlignment(Pos.CENTER);
        header.setPadding(new Insets(0, 0, 20, 0));

        return header;
    }

    private VBox createLoginForm() {
        VBox formContainer = new VBox(20);
        formContainer.setStyle(
                "-fx-background-color: #2d2d2d; " +
                        "-fx-padding: 30; " +
                        "-fx-background-radius: 10; " +
                        "-fx-border-radius: 10; " +
                        "-fx-border-color: #3d3d3d; " +
                        "-fx-max-width: 320;"
        );
        formContainer.setAlignment(Pos.CENTER);

        // Username field
        usernameField = createStyledTextField("Username");
        VBox usernameContainer = createFormField("Username", usernameField);

        // Password field
        passwordField = new PasswordField();
        passwordField.setPromptText("Password");
        passwordField.setStyle(
                "-fx-background-color: #2d2d2d; " +
                        "-fx-text-fill: white; " +
                        "-fx-prompt-text-fill: #808080; " +
                        "-fx-border-color: #3d3d3d; " +
                        "-fx-border-radius: 5; " +
                        "-fx-background-radius: 5; " +
                        "-fx-padding: 8;"
        );
        VBox passwordContainer = createFormField("Password", passwordField);

        // Login button
        Button loginButton = createStyledButton("Login/Register", "#1e4620");
        loginButton.setMaxWidth(Double.MAX_VALUE);
        loginButton.setOnAction(e -> handleLogin());

        // "Forgot Password" link
        Hyperlink forgotPasswordLink = new Hyperlink("Forgot Password?");
        forgotPasswordLink.setStyle(
                "-fx-text-fill: #4a9eff; " +
                        "-fx-border-color: transparent;"
        );
        forgotPasswordLink.setOnAction(e -> handleForgotPassword());

        // Add all elements to form container
        formContainer.getChildren().addAll(
                usernameContainer,
                passwordContainer,
                loginButton,
                forgotPasswordLink
        );

        return formContainer;
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

    private Button createStyledButton(String text, String color) {
        Button button = new Button(text);
        button.setStyle(
                "-fx-background-color: " + color + "; " +
                        "-fx-text-fill: white; " +
                        "-fx-font-size: 14px; " +
                        "-fx-padding: 12 20 12 20; " +
                        "-fx-background-radius: 5; " +
                        "-fx-cursor: hand;"
        );
        return button;
    }

    private VBox createFormField(String labelText, Control field) {
        Label label = new Label(labelText);
        label.setStyle(
                "-fx-font-weight: bold; " +
                        "-fx-text-fill: #bebebe;"
        );
        VBox container = new VBox(5, label, field);
        return container;
    }

    private void handleLogin() {
        String username = usernameField.getText().trim();
        String password = passwordField.getText();

        if (username.isEmpty() || password.isEmpty()) {
            showAlert("Error", "Please enter both username and password.");
            return;
        }

        try {
            // Call native method to validate user
            boolean isValid = validateUser(username, password);

            if (isValid) {
                // Launch main application
                ShareItApp mainApp = new ShareItApp();
                mainApp.start(new Stage());

                // Close login window
                primaryStage.close();
            } else {
                showAlert("Error", "Invalid username or password.");
                passwordField.clear();
            }
        } catch (Exception e) {
            showAlert("Error", "Login failed. Please try again later.");
            e.printStackTrace();
        }
    }

    private void handleForgotPassword() {
        showAlert("Info", "Please contact your system administrator to reset your password.");
    }

    private void showAlert(String title, String content) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle(title);
        alert.setHeaderText(null);
        alert.setContentText(content);
        alert.getDialogPane().setStyle(
                "-fx-background-color: #ffffff;" +
                        "-fx-text-fill: white;"
        );

        // Style the dialog buttons
        alert.getDialogPane().lookupButton(ButtonType.OK)
                .setStyle("-fx-background-color: #1e4620; -fx-text-fill: white;");

        alert.showAndWait();
    }

    public static void main(String[] args) {
        launch(args);
    }
}