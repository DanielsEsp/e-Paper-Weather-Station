



const unsigned char* icone_name[34] = { epd_bitmap_clair_jour, epd_bitmap_clair_nuit,
                                        epd_bitmap_nuages,
                                        epd_bitmap_nuages_jour, epd_bitmap_nuages_nuit,
                                        epd_bitmap_couvert, epd_bitmap_brouillard,
                                        epd_bitmap_brouillard_verglas, epd_bitmap_brouillard,
                                        epd_bitmap_brouillard, epd_bitmap_brouillard,
                                        epd_bitmap_brouillard_verglas, epd_bitmap_brouillard_verglas,
                                        epd_bitmap_averses_jour, epd_bitmap_averses_nuit,
                                        epd_bitmap_averses_jour, epd_bitmap_averses_nuit,
                                        epd_bitmap_averses_jour, epd_bitmap_averses_nuit,
                                        epd_bitmap_averses, epd_bitmap_averses,
                                        epd_bitmap_pluie, epd_bitmap_pluie_verglas,
                                        epd_bitmap_pluie_verglas, epd_bitmap_pluie,
                                        epd_bitmap_neige, epd_bitmap_neige,
                                        epd_bitmap_neige, epd_bitmap_neige,
                                        epd_bitmap_giboulles, epd_bitmap_giboulles,
                                        epd_bitmap_orage, epd_bitmap_orage, 
                                        epd_bitmap_grele }; 
                                                                                                                      
String icone_datas[34] = { "0", "0", "1", "2", "2", "3", "45", "48", "51", "53", "55", "56", "57",
                          "61", "61", "63", "63", "80", "80", "81", "82", "65", "66", "67", "82",
                          "71", "73", "75", "77", "85", "86", "95", "96", "99" };

String data_description[34] = { "Journée ciel clair", "Nuit claire", 
                                "Couvert",
                                "Journée principalement claire", "Nuit principalement claire", 
                                "Brume", "Brouillard", 
                                "Brouillard givrant", "Légère bruine", 
                                "Bruine moderée", "Bruine dense",
                                "Bruine verglaçante", "Forte bruine verglaçante",
                                "Légère pluie la journée", "Légère pluie la nuit",
                                "Pluie modérée la journée", "Pluie modérée la nuit",
                                "Faibles averses la journée", "Faibles averses la nuit",
                                "Averses modérées", "Violentes averses",
                                "Forte pluie", "Légère pluie verglaçante",     
                                "Forte pluie verglaçante", "Violentes averses",
                                "Légères chutes de neige", "Chutes de neige moderées",
                                "Fortes chutes de neige", "Neige",             
                                "Légères averses de neige", "Fortes averses de neige", 
                                "Orage faible ou moderé", "Orage faible risque de grêle",
                                "Orage et risque de grêle" };



//
