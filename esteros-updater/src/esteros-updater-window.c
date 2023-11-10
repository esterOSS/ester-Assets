/* esteros-updater-window.c
 *
 * Copyright 2023 JukFiuu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "config.h"

#include "esteros-updater-window.h"

#include <gio/gio.h>
#include <string.h>


struct _EsterosUpdaterWindow {
  AdwApplicationWindow parent_instance;

  /* Template widgets */
  GtkHeaderBar * header_bar;
  GtkLabel * label;
  GtkButton * mainbutton;
  GtkLabel * statustext;
  GtkLabel * osname;
  GtkLabel * osdescription;
  GtkLabel * ossize;
  GtkImage * osicon;
};

G_DEFINE_FINAL_TYPE(EsterosUpdaterWindow, esteros_updater_window, ADW_TYPE_APPLICATION_WINDOW)

static void
esteros_updater_window_class_init(EsterosUpdaterWindowClass * klass) {
  GtkWidgetClass * widget_class = GTK_WIDGET_CLASS(klass);

  gtk_widget_class_set_template_from_resource(widget_class, "/co/uk/esteros/Updater/esteros-updater-window.ui");
  gtk_widget_class_bind_template_child(widget_class, EsterosUpdaterWindow, header_bar);
  gtk_widget_class_bind_template_child(widget_class, EsterosUpdaterWindow, label);
  gtk_widget_class_bind_template_child(widget_class, EsterosUpdaterWindow, mainbutton);
  gtk_widget_class_bind_template_child(widget_class, EsterosUpdaterWindow, statustext);
  gtk_widget_class_bind_template_child(widget_class, EsterosUpdaterWindow, osname);
  gtk_widget_class_bind_template_child(widget_class, EsterosUpdaterWindow, ossize);
  gtk_widget_class_bind_template_child(widget_class, EsterosUpdaterWindow, osdescription);
  gtk_widget_class_bind_template_child(widget_class, EsterosUpdaterWindow, osicon);
}

static void esteros_updater_window__install(GtkWidget * widget, gpointer data) {
  EsterosUpdaterWindow *self = ESTEROS_UPDATER_WINDOW(data);
  gtk_widget_set_sensitive(widget, false);
  gtk_label_set_text(self -> statustext, "Updating in progress...");
  gtk_label_set_text(self -> osname, "");
  gtk_label_set_text(self -> ossize, "");
  gtk_label_set_text(self -> osdescription, "");
  gtk_image_clear(GTK_IMAGE(self->osicon));
  // Download command
  char * wgetInfoCommand = "wget -O info.txt https://raw.githubusercontent.com/esterOSS/updates/main/latest/info.txt";
  int status = system(wgetInfoCommand);

  if (status == 0) {
    // Download was successful, now read and update the UI
    GError * error = NULL;
    gchar * infoContent = NULL;
    GFile * infoFile = g_file_new_for_path("info.txt");

    if (g_file_load_contents(infoFile, NULL, & infoContent, NULL, NULL, & error)) {
      // Parse the info content and update the UI elements
      GKeyFile * infoKeyFile = g_key_file_new();
      g_key_file_load_from_data(infoKeyFile, infoContent, -1, G_KEY_FILE_NONE, & error);

      if (error == NULL) {
        const char * scriptUrl = g_key_file_get_string(infoKeyFile, "Info", "Script", NULL);
        char wgetScriptCommand[256] = "wget -O update.sh ";
        strcat(wgetScriptCommand,scriptUrl);
        int updateDlStatus = system(wgetScriptCommand);
        // Run
        if(updateDlStatus == 0){
          system("chmod +x update.sh");
          int updateStatus = system("./update.sh");
          if(updateStatus == 0){
            gtk_label_set_text(self -> statustext, "Updating complete! Please restart!");
          }else{
            gtk_label_set_text(self -> statustext, "Updating failed! Please report this to the devs.");
          }
        }else{
          gtk_label_set_text(self -> statustext, "Downloading failed!");
        }
      } else {
        g_print("Failed to parse info.txt: %s\n", error -> message);
      }
    } else {
      g_print("Failed to read info.txt: %s\n", error -> message);
    }

    g_object_unref(infoFile);
    //gtk_widget_set_sensitive (widget, true);
  } else {
    g_print("Failed to download info.txt: wget exited with non-zero status\n");
  }
}

static void
esteros_updater_window_init(EsterosUpdaterWindow * self) {
  gtk_widget_init_template(GTK_WIDGET(self));
  g_autoptr(GSimpleAction) install_action = g_simple_action_new("install", NULL);
  g_signal_connect(self -> mainbutton, "clicked", G_CALLBACK(esteros_updater_window__install), self);
  g_action_map_add_action(G_ACTION_MAP(self), G_ACTION(install_action));
  //const char *infoUrl = "https://raw.githubusercontent.com/esterOSS/updates/main/latest/info.txt";
  // "An update is available"
  gtk_label_set_text(self -> statustext, "An update is available!");
  // Download command
  char * wgetInfoCommand = "wget -O info.txt https://raw.githubusercontent.com/esterOSS/updates/main/latest/info.txt";

  int status = system(wgetInfoCommand);

  if (status == 0) {
    // Download was successful, now read and update the UI
    GError * error = NULL;
    gchar * infoContent = NULL;
    GFile * infoFile = g_file_new_for_path("info.txt");

    if (g_file_load_contents(infoFile, NULL, & infoContent, NULL, NULL, & error)) {
      // Parse the info content and update the UI elements
      GKeyFile * infoKeyFile = g_key_file_new();
      g_key_file_load_from_data(infoKeyFile, infoContent, -1, G_KEY_FILE_NONE, & error);

      if (error == NULL) {
        const char * name = g_key_file_get_string(infoKeyFile, "Info", "Name", NULL);
        const char * description = g_key_file_get_string(infoKeyFile, "Info", "Description", NULL);
        const char * iconUrl = g_key_file_get_string(infoKeyFile, "Info", "Icon", NULL);
        const char * year = g_key_file_get_string(infoKeyFile, "Info", "Year", NULL);
        const char * month = g_key_file_get_string(infoKeyFile, "Info", "Month", NULL);
        const char * day = g_key_file_get_string(infoKeyFile, "Info", "Day", NULL);
        char bdate[256] = "\0";
        int pleasedontmakemekysignorethispls = 0;

        for (pleasedontmakemekysignorethispls = 0; pleasedontmakemekysignorethispls < 4; pleasedontmakemekysignorethispls++) {
            bdate[pleasedontmakemekysignorethispls] = year[pleasedontmakemekysignorethispls];
        }
        bdate[pleasedontmakemekysignorethispls] = '.';
        pleasedontmakemekysignorethispls++;

        for (int j = 0; j < 2; j++) {
            bdate[pleasedontmakemekysignorethispls] = month[j];
            pleasedontmakemekysignorethispls++;
        }
        bdate[pleasedontmakemekysignorethispls] = '.';
        pleasedontmakemekysignorethispls++;

        for (int j = 0; j < 2; j++) {
            bdate[pleasedontmakemekysignorethispls] = day[j];
            pleasedontmakemekysignorethispls++;
        }
        
        // Update the UI elements with the parsed values
        if (name) {
          gtk_label_set_text(self -> osname, name);
        }
        if(bdate){
          gtk_label_set_text(self -> ossize, bdate);
        }

        if (description) {
          gtk_label_set_text(self -> osdescription, description);
        }

        if (iconUrl) {
            char wgetImageCommand[256] = "wget -O osimage.svg ";
            strcat(wgetImageCommand, iconUrl);
            int imgStatus = system(wgetImageCommand);

            if (imgStatus == 0) {
              // Download was successful, now set the image in GtkImage
              const char *imageFilePath = "osimage.svg";
              gtk_image_clear(GTK_IMAGE(self->osicon));
              gtk_image_set_from_file(GTK_IMAGE(self->osicon), imageFilePath);
            } else {
              g_print("Failed to download image: wget exited with non-zero status\n");
            }
        }
      } else {
        g_print("Failed to parse info.txt: %s\n", error -> message);
      }
    } else {
      g_print("Failed to read info.txt: %s\n", error -> message);
    }

    g_object_unref(infoFile);
  } else {
    g_print("Failed to download info.txt: wget exited with non-zero status\n");
  }
}
