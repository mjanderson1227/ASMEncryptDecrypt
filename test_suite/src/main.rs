use std::process::Command;
use file_diff::diff;

fn encrypt(filename: &str) {
    Command::new("../../ComputerOrganizationProject/Debug/ComputerOrganizationProject.exe")
        .args(["-e", fmt!("../../ComputerOrganizationProject/Resources/TestFiles/{}", filename), "-k", "../../ComputerOrganizationProject/Resources/Key.dat", "-o", "../../ComputerOrganizationProject/Resources/output.enc"])
        .output()
        .expect("Could not find file!");

    if diff(fmt!("../../ComputerOrganizationProject/Resources/TestFiles/{}", filename), fmt!("../../ComputerOrganizationProject/Resources/DecryptedFiles/{}", filename)).is_ok() {
        println!("Files are the same!");
    } else {
        println!("Files are different!"); }
    
}

fn decrypt() {
    Command::new("../../ComputerOrganizationProject/Debug/ComputerOrganizationProject.exe")
        .args(["-d", "../../ComputerOrganizationProject/Resources/output.enc", "-k", "../../ComputerOrganizationProject/Resources/Key.dat", "-o", "../../ComputerOrganizationProject/Resources/TestFiles"])
        .output()
        .expect("Could not find file!");
}

fn main() {
    // Get user option:
    let mut choice = String::new();
    print!("Choose an option:\n1. Encryption\n2. Decryption\n");
    std::io::stdin()
        .read_line(&mut choice)
        .expect("Unable to read line");

    let choice: u32 = choice.trim().parse().expect("Error: Please type a number");

    match choice {
        1 => decrypt(),
        2 => encrypt(),
        _ => panic!("Please Pick a Number between 1 and 2")
    };
}
