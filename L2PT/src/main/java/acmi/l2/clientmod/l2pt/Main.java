/**
 * Copyright 2016 acmi
 * <p>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * <p>
 * http://www.apache.org/licenses/LICENSE-2.0
 * <p>
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package acmi.l2.clientmod.l2pt;

import acmi.l2.clientmod.unreal.Environment;
import javafx.application.Application;
import javafx.beans.binding.Bindings;
import javafx.beans.property.*;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.stage.Stage;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.net.URISyntaxException;
import java.nio.file.Paths;
import java.util.ResourceBundle;
import java.util.jar.JarFile;
import java.util.jar.Manifest;
import java.util.logging.Level;
import java.util.logging.LogManager;
import java.util.logging.Logger;
import java.util.prefs.Preferences;

public class Main extends Application {
    private static final Logger log = Logger.getLogger(Main.class.getName());

    private ObjectProperty<Stage> stage = new SimpleObjectProperty<>(this, "stage");

    private StringProperty applicationName = new SimpleStringProperty(this, "applicationName", "L2PT");
    private StringProperty version = new SimpleStringProperty(this, "version");
    private StringProperty jreVersion = new SimpleStringProperty(this, "jreVersion");
    private StringProperty jvmVersion = new SimpleStringProperty(this, "jvmVersion");

    private ObjectProperty<Environment> environment = new SimpleObjectProperty<>(this, "environment");

    public Stage getStage() {
        return stage.get();
    }

    public ReadOnlyObjectProperty<Stage> stageProperty() {
        return stage;
    }

    private void setStage(Stage stage) {
        this.stage.set(stage);
    }

    public String getApplicationName() {
        return applicationName.get();
    }

    public ReadOnlyStringProperty applicationNameProperty() {
        return applicationName;
    }

    public String getVersion() {
        return version.get();
    }

    public ReadOnlyStringProperty versionProperty() {
        return version;
    }

    private void setVersion(String version) {
        this.version.set(version);
    }

    public String getJreVersion() {
        return jreVersion.get();
    }

    public ReadOnlyStringProperty jreVersionProperty() {
        return jreVersion;
    }

    private void setJreVersion(String jreVersion) {
        this.jreVersion.set(jreVersion);
    }

    public String getJvmVersion() {
        return jvmVersion.get();
    }

    public ReadOnlyStringProperty jvmVersionProperty() {
        return jvmVersion;
    }

    private void setJvmVersion(String jvmVersion) {
        this.jvmVersion.set(jvmVersion);
    }

    public Environment getEnvironment() {
        return environment.get();
    }

    public ObjectProperty<Environment> environmentProperty() {
        return environment;
    }

    public void setEnvironment(Environment environment) {
        this.environment.set(environment);
    }

    @Override
    public void start(Stage stage) throws Exception {
        setStage(stage);

        setVersion(readAppVersion());
        setJreVersion(System.getProperty("java.version"));
        setJvmVersion(System.getProperty("java.vm.name") + " by " + System.getProperty("java.vendor"));

        FXMLLoader loader = new FXMLLoader(getClass().getResource("l2pt.fxml"));
        loader.setResources(ResourceBundle.getBundle("acmi.l2.clientmod.l2pt.l2pt"));

        stage.setScene(new Scene(loader.load()));
        stage.getIcons().add(new Image(Main.class.getResourceAsStream("l2pt.png")));
        stage.titleProperty().bind(Bindings.createStringBinding(() ->
                        (getEnvironment() != null ? getEnvironment().getStartDir().getAbsolutePath() + " - " : "") +
                                getApplicationName() + " " + getVersion(),
                environmentProperty()));

        L2PT controller = loader.getController();
        controller.setApplication(this);

        stage.show();
    }

    private String readAppVersion() throws IOException, URISyntaxException {
        try (JarFile jarFile = new JarFile(Paths.get(getClass().getProtectionDomain().getCodeSource().getLocation().toURI()).toFile())) {
            Manifest manifest = jarFile.getManifest();
            return manifest.getMainAttributes().getValue("Version");
        } catch (FileNotFoundException ignore) {
        } catch (IOException | URISyntaxException e) {
            log.log(Level.WARNING, "version info load error", e);
        }
        return "";
    }

    public static Preferences getPrefs() {
        return Preferences.userRoot().node("l2pt");
    }

    public static void main(String[] args) {
        try (InputStream is = Main.class.getResourceAsStream("logging.properties")) {
            LogManager.getLogManager().readConfiguration(is);
        } catch (IOException e) {
            System.err.println("Couldn't load logging.properties");
        }

        launch(args);
    }
}
