use std::path::Path;
use std::fs;
use std::process::Command;
use file_diff::*;

fn list_options(path: &Path) {
    let dir_iter = match fs::read_dir(path) {
        Err(why) => panic!("Error: {}", why),
        Ok(value) => value
    };

    let text_iter = dir_iter.map(|x| x.unwrap().file_name());


    let mut menu = youchoose::Menu::new(text_iter);
    let option = menu.show();
}

fn exec_json(json: &Path) {
    use serde_json::{ Value, from_reader };
    let disp = json.display();
    
    let file = match fs::File::open(json) {
        Err(why) => panic!("Couldn't open: {}, {}", disp, why),
        Ok(file) => file
    };

    let file_buffer = std::io::BufReader::new(file);

    let json_data: Value = from_reader(file_buffer).expect("Unable to read from json file");

    Command::new(json_data["executable"].as_str().unwrap())
        .arg(json_data["target"].as_str().unwrap())
        .arg(json_data["key"].as_str().unwrap())
        .arg(json_data["password"].as_str().unwrap())
        .arg(json_data["output"].as_str().unwrap())
        .output()
        .expect("Unable to execute command");
}


fn main() {
    let settings_path = Path::new("settings.json");
    exec_json(settings_path);
}
