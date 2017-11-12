#include <gst/gst.h>
#include <glib.h>


int
main (int   argc,
      char *argv[])
{
  //GMainLoop *loop;
  bool terminate=false;
  GstMessage *msg;

  GstElement *pipeline, *source, *parser, *decoder, *conv, *sink;
  GstBus *bus;
  //guint bus_watch_id;

  /* Initialisation */
  gst_init (&argc, &argv);

  //loop = g_main_loop_new (NULL, FALSE);

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
  
  // Add elements to pipeline
  gst_bin_add_many (GST_BIN(pipeline), source, parser, decoder, conv, sink, NULL);

  // Link the elements together
  gst_element_link_many(source, parser, decoder, conv, sink, NULL);

  // Set pipeline to playing
  g_print ("Now playing: %s\n", "default");
  gst_element_set_state (pipeline, GST_STATE_PLAYING);


  // Loop---------------------------------------------
  g_print ("Running...\n");
  bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  do {
    // Pop one by one
    msg = gst_bus_timed_pop (bus, GST_CLOCK_TIME_NONE);

    /* Parse message */
    if (msg != NULL) {
      GError *err;
      gchar *debug_info;
      
      switch (GST_MESSAGE_TYPE (msg)) {
      case GST_MESSAGE_ERROR:
	gst_message_parse_error (msg, &err, &debug_info);
	g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
	g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
	g_clear_error (&err);
	g_free (debug_info);
	terminate = true;
	break;
      case GST_MESSAGE_EOS:
	g_print ("End-Of-Stream reached.\n");
	terminate = true;
	break;
      case GST_MESSAGE_STATE_CHANGED:
	/* We are only interested in state-changed messages from the pipeline */
	if (GST_MESSAGE_SRC (msg) == GST_OBJECT (pipeline)) {
	  GstState old_state, new_state, pending_state;
	  gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
	  g_print ("Pipeline state changed from %s to %s:\n",
		   gst_element_state_get_name (old_state), gst_element_state_get_name (new_state));
	}
	break;
      default:
	// Do not handle any other message
	g_printerr ("INFO: unhandled message\n");
	break;
      }
      gst_message_unref (msg);
    }
  } while (!terminate);
  
  
  

  // Deinitialization
  g_print ("Returned, stopping playback\n");
  gst_object_unref (bus);
  gst_element_set_state (pipeline, GST_STATE_NULL);

  g_print ("Deleting pipeline\n");
  gst_object_unref (GST_OBJECT (pipeline));
  

  return 0;
}
