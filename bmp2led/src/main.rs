use bmp::Pixel;
use std::{
    env,
    io::{stdout, Write},
    process::exit,
};

fn main() {
    let filename: Vec<_> = env::args().skip(1).collect();
    if filename.len() != 1 {
        println!("Nur ein Argument erwartet: Der bmp-Dateiname");
        exit(0);
    }
    let filename = filename.into_iter().next().unwrap();

    let img = bmp::open(filename).unwrap_or_else(|error| {
        println!(".bmp-Datei konnte nicht geöffnet werden: {}", error);
        exit(0);
    });

    if img.get_height() != 8 || img.get_width() % 6 != 5 {
        println!(
            "\
Das Bild muss für jede LED einen Pixel verwenden.
Die einzelnen Animationszustände müssen hochkant gezeichnet werden.
Zwischen 2 Animationszuständen muss immer 1 Pixel breit Abstand gelassen werden.
Die Farbe dieser Abstand-Pixel ist egal."
        );
        exit(0);
    }

    let states: Vec<Vec<Pixel>> = (0..=img.get_width() / 6)
        .map(|state_index| {
            (state_index * 6..state_index * 6 + 5)
                .flat_map(|width_index| {
                    (0..8)
                        .map(move |height_index| (width_index, height_index))
                        .map(|(x, y)| img.get_pixel(x, y))
                })
                .collect()
        })
        .collect();

    if states.len() == 1 {
        print_state(&states.into_iter().next().unwrap());
        stdout().flush().unwrap();
        exit(0);
    }

    print!("{{");
    for (i, state) in states.iter().enumerate() {
        print_state(state);
        if i != states.len() - 1 {
            print!(",");
        }
    }
    print!("}}");
    stdout().flush().unwrap();
    exit(0);
}

fn print_state(state: &Vec<Pixel>) {
    print!("{{");
    for (i, pixel) in state.iter().enumerate() {
        print!("pix{{{},{},{}}}", pixel.r, pixel.g, pixel.b);
        if i != state.len() - 1 {
            print!(",");
        }
    }
    print!("}}");
}
