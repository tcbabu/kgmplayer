#define D_KULINA
#ifdef D_KULINA
int kgffmpeg(int ,char **);
int ffmpegfun(int argc, char **argv);

int main(int argc,char **argv) {
  return kgffmpeg(argc,argv);
}
#if 0
int main(int argc, char **argv) {
   return kgffmpeg( argc,argv);
}
#endif
#if 0
int ffmpegfun(int argc, char **argv)
{
    int ret;
    int64_t ti;
    printf("Inside ffmpegfun\n");
    fflush(stdout);
    register_exit(ffmpeg_cleanup);
//    setvbuf(stderr,NULL,_IONBF,0); /* win32 runtime needs this */

    av_log_set_flags(AV_LOG_SKIP_REPEATED);
    parse_loglevel(argc, argv, options);
    if(argc>1 && !strcmp(argv[1], "-d")){
        run_as_daemon=1;
        av_log_set_callback(log_callback_null);
        argc--;
        argv++;
    }

    avcodec_register_all();
#if CONFIG_AVDEVICE
    avdevice_register_all();
#endif
    avfilter_register_all();
    av_register_all();
    avformat_network_init();

//    show_banner(argc, argv, options);

//    term_init();

    /* parse options and open all input/output files */
    ret = ffmpeg_parse_options(argc, argv);
    if (ret < 0)
        return 0;
#if 0
    if (nb_output_files <= 0 && nb_input_files == 0) {
        show_usage();
        av_log(NULL, AV_LOG_WARNING, "Use -h to get full help or, even better, run 'man %s'\n", program_name);
        exit_program(1);
    }

    /* file converter / grab */
    if (nb_output_files <= 0) {
        av_log(NULL, AV_LOG_FATAL, "At least one output file must be specified\n");
        exit_program(1);
    }

#endif
//     if (nb_input_files == 0) {
//         av_log(NULL, AV_LOG_FATAL, "At least one input file must be specified\n");
//         exit_program(1);
//     }

    current_time = ti = getutime();
    if (transcode() < 0)
        exit_program(1);
    ti = getutime() - ti;
    if (do_benchmark) {
        av_log(NULL, AV_LOG_INFO, "bench: utime=%0.3fs\n", ti / 1000000.0);
    }
#if 0
    av_log(NULL, AV_LOG_DEBUG, "%"PRIu64" frames successfully decoded, %"PRIu64" decoding errors\n",
           decode_error_stat[0], decode_error_stat[1]);
    if ((decode_error_stat[0] + decode_error_stat[1]) * max_error_rate < decode_error_stat[1])
        exit_program(69);
    exit_program(received_nb_signals ? 255 : main_return_code);
#endif
    return 1;
}
#endif
#endif
