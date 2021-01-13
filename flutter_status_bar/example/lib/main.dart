import 'package:flutter/material.dart';
import 'package:flutter_status_bar/flutter_status_bar.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  @override
  void initState() {
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: Center(
          child: Column(
            children: [
              TextButton(
                  onPressed: () {
                    FlutterStatusBar.showStatusBar('FlutterStatusBar')
                        .then((value) => print('showStatusBar $value'));
                  },
                  child: Text('showStatusBar')),
              TextButton(
                  onPressed: () {
                    FlutterStatusBar.setStatusBarText('hello world')
                        .then((value) => print('setStatusBarText $value'));
                  },
                  child: Text('setStatusBarText')),
              TextButton(
                  onPressed: () {
                    FlutterStatusBar.isShown()
                        .then((value) => print('isShown $value'));
                  },
                  child: Text('isShown')),
              TextButton(
                  onPressed: () {
                    FlutterStatusBar.hideStatusBar()
                        .then((value) => print('hideStatusBar $value'));
                  },
                  child: Text('hideStatusBar'))
            ],
          ),
        ),
      ),
    );
  }
}
