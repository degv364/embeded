#include <gst/gst.h>
#include <glib.h>

// Message handler callback
static gboolean
bus_call (GstBus     *bus,
          GstMessage *msg,
          gpointer    data)
{
  GMainLoop *loop = (GMainLoop *) data;

  switch (GST_MESSAGE_TYPE (msg)) {

    case GST_MESSAGE_EOS:
      g_print ("End of stream\n");
      g_main_loop_quit (loop);
      break;

    case GST_MESSAGE_ERROR: {
      gchar  *debug;
      GError *error;

      gst_message_parse_error (msg, &error, &debug);
      g_free (debug);

      g_printerr ("Error: %s\n", error->message);
      g_error_free (error);

      g_main_loop_quit (loop);
      break;
    }
    default:
      break;
  }

  return TRUE;
}


int
main (int   argc,
      char *argv[])
{
  GMainLoop *loop;

  GstElement *pipeline, *source, *parser, *decoder, *conv, *sink;
  GstBus *bus;
  guint bus_watch_id;

  /* Initialisation */
  gst_init (&argc, &argv);

  loop = g_main_loop_new (NULL, FALSE);

  /* Create gstreamer elements */
  pipeline = gst_pipeline_new("mp3_player");
  source   = gst_element_factory_make ("filesrc",       "file-source");
  parser   = gst_element_factory_make ("mpegaudioparse", "parser");
  decoder  = gst_element_factory_make ("mpg123audiodec",     "auto-decoder");
  conv     = gst_element_factory_make ("audioconvert",  "converter");
  sink     = gst_element_factory_make ("pulsesink", "audio-output");

  if (!pipeline || !source || !parser || !decoder || !conv ||!sink) {
    g_printerr ("One element could not be created. Exiting.\n");
    return -1;
  }

  /* Set up the pipeline */

  /* we set the input filename to the source element */
  g_object_set (G_OBJECT (source), "location",
		"../../media/music/Tours_-_01_-_Enthusiast.mp3", NULL);

  // Message handler
  bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  bus_watch_id = gst_bus_add_watch (bus, bus_call, loop);
  gst_object_unref (bus);

  // Add elements to pipeline
  gst_bin_add_many (GST_BIN(pipeline), source, parser, decoder, conv, sink, NULL);

  // Link the elements together
  gst_element_link_many(source, parser, decoder, conv, sink, NULL);

  // Set pipeline to playing
  g_print ("Now playing: %s\n", "default");
  gst_element_set_state (pipeline, GST_STATE_PLAYING);


  // Loop
  g_print ("Running...\n");
  g_main_loop_run (loop);


  // Deinitialization
  g_print ("Returned, stopping playback\n");
  gst_element_set_state (pipeline, GST_STATE_NULL);

  g_print ("Deleting pipeline\n");
  gst_object_unref (GST_OBJECT (pipeline));
  g_source_remove (bus_watch_id);
  g_main_loop_unref (loop);

  return 0;
}
