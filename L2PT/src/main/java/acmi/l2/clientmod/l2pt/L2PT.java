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

import javafx.application.Platform;
import javafx.beans.property.ObjectProperty;
import javafx.beans.property.ReadOnlyObjectProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.fxml.Initializable;
import javafx.geometry.Insets;
import javafx.scene.control.*;
import javafx.scene.layout.VBox;
import javafx.stage.StageStyle;

import java.net.URL;
import java.util.ResourceBundle;

public class L2PT implements Initializable {
    private ObjectProperty<Main> application = new SimpleObjectProperty<>(this, "application");

    private ObjectProperty<ResourceBundle> resources = new SimpleObjectProperty<>(this, "resources");

    public Main getApplication() {
        return application.get();
    }

    public ObjectProperty<Main> applicationProperty() {
        return application;
    }

    public void setApplication(Main application) {
        this.application.set(application);
    }

    public ResourceBundle getResources() {
        return resources.get();
    }

    public ReadOnlyObjectProperty<ResourceBundle> resourcesProperty() {
        return resources;
    }

    private void setResources(ResourceBundle resources) {
        this.resources.set(resources);
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        setResources(resources);
    }

    public void exit() {
        Platform.exit();
    }

    public void showAbout() {
        Dialog dialog = new Dialog();
        dialog.initStyle(StageStyle.UTILITY);
        dialog.setTitle(getResources().getString("help.about"));

        Label application = new Label(getApplication().getApplicationName() + " " + getApplication().getVersion());
        Label jre = new Label("JRE: " + getApplication().getJreVersion());
        Label jvm = new Label("JVM: " + getApplication().getJvmVersion());
        Hyperlink link = new Hyperlink("GitHub");
        link.setOnAction(event -> getApplication().getHostServices().showDocument("https://github.com/acmi/L2PT"));

        VBox content = new VBox(application, jre, jvm, link);
        VBox.setMargin(jre, new Insets(10, 0, 0, 0));
        VBox.setMargin(link, new Insets(10, 0, 0, 0));

        DialogPane pane = new DialogPane();
        pane.setContent(content);
        pane.getButtonTypes().addAll(ButtonType.OK);
        dialog.setDialogPane(pane);

        dialog.showAndWait();
    }
}
