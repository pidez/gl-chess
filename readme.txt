### funzionalità di picking ###

Il rendering della scena viene effettuato due volte: la prima volta assegnando un colore univoco a ciascun oggetto
e la seconda volta con i colori naturali. Il colore univoco viene calcolato a partire dall'indice della mesh in modo
da accoppiare colore e id della mesh considerata. Quando viene rilevato un click del mouse inizia la fase di rendering:
viene renderizzata la scena con i colori univoci e viene salvato il colore del pixel su cui è avvenuto il click. Successivamente
si renderizza la scena con i colori naturali e si ottiene l'id dell'oggetto clickato a partire dal colore appena salvato.