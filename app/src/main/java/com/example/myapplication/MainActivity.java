package com.example.myapplication;

import androidx.appcompat.app.AppCompatActivity;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import java.io.*;


public class MainActivity extends AppCompatActivity {


    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
        System.loadLibrary("node");
    }

    //We just want one instance of node running in the background.
    public static boolean _startedNodeAlready=false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        copyAssetsToInternalStorage(this);
        if( !_startedNodeAlready ) {
            _startedNodeAlready=true;
            new Thread(new Runnable() {
                @Override
                public void run() {
                    System.out.println("Response from server:---------------------------------- ");
                    startNodeWithArguments(new String[]{"node",
                            "/data/user/0/com.example.myapplication/files/httpTest.js"
//                            "-e",
//                            "console.log('22222222222222222');" +
//                                    "var http = require('http'); " +
//                                    "var versions_server = http.createServer( (request, response) => { " +
//                                    "  response.end('Versions: ' + JSON.stringify(process.versions)); " +
//                                    "}); " +
//                                    "versions_server.listen(3000,'0.0.0.0');"
                    });
                    System.out.println("Response from server:-111111111111111111111111111111111111111 ");
                }

            }).start();
        };

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native Integer startNodeWithArguments(String[] arguments);





    public static void copyAssetsToInternalStorage(Context context) {
        File targetDir = context.getFilesDir();
        try {
            // 获取assets中的所有文件和文件夹
            String[] files = context.getAssets().list("");
            if (files != null) {
                for (String file : files) {
                    copyAsset(context, file, targetDir);
                }
            }
        } catch (IOException e) {
            Log.e("TAG", "Error copying assets", e);
        }
    }

    private static void copyAsset(Context context, String assetPath, File targetDir) throws IOException {
        File outFile = new File(targetDir, assetPath);
        if (assetPath.contains("/")) {
            // If path contains '/', create directories
            outFile.getParentFile().mkdirs();
        }

        if (context.getAssets().list(assetPath).length > 0) {
            // If it's a directory
            outFile.mkdirs();
            String[] files = context.getAssets().list(assetPath);
            if (files != null) {
                for (String file : files) {
                    copyAsset(context, assetPath + "/" + file, targetDir);
                }
            }
        } else {
            // If it's a file
            try (InputStream in = context.getAssets().open(assetPath);
                 FileOutputStream out = new FileOutputStream(outFile)) {
                byte[] buffer = new byte[1024];
                int read;
                while ((read = in.read(buffer)) != -1) {
                    out.write(buffer, 0, read);
                }
            }
        }
    }
}

