private fun isNetworkConnected(): Boolean {
  //1
  val connectivityManager = getSystemService(Context.CONNECTIVITY_SERVICE) as ConnectivityManager 
  //2
  val activeNetwork = connectivityManager.activeNetwork
  //3
  val networkCapabilities = connectivityManager.getNetworkCapabilities(activeNetwork)
  //4 
  return networkCapabilities != null && 
      networkCapabilities.hasCapability(NetworkCapabilities.NET_CAPABILITY_INTERNET) 
}
// This code to be written on MainActivity.kt
