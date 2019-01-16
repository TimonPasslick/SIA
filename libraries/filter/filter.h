//ignoriert extreme Abweichungen nach oben
template <typename Signal, typename Alpha>
inline Signal emaTiefpass(const Signal sensorwert, const Alpha alpha, Signal& s) {
  return s = alpha * sensorwert + (1 - alpha) * s;
}


//ignoriert extreme Abweichungen nach unten
template <typename Signal, typename Alpha>
inline Signal emaHochpass(const Signal sensorwert, const Alpha alpha, Signal& s) {
  return sensorwert - emaTiefpass(s, alpha, sensorwert);
}

//ignoriert extreme Abweichungen zu beiden Seiten
template <typename Signal, typename Alpha>
inline Signal emaBandpass(
  const Signal sensorwert,
  const Alpha alphaTief, const Alpha alphaHoch,
  Signal& sTief,         Signal& sHoch
) {
  return emaTiefpass(sensorwert, alphaHoch, sHoch)
       - emaTiefpass(sensorwert, alphaTief, sTief);
}

//ignoriert kleine Abweichungen
template <typename Signal, typename Alpha>
inline Signal emaBandstopp(
  const Signal sensorwert,
  const Alpha alphaTief, const Alpha alphaHoch,
  Signal& sTief,         Signal& sHoch
) {
  return sensorwert
    - emaBandpass(
      sensorwert,
      alphaTief, alphaHoch,
      sTief, sHoch
    );
}
