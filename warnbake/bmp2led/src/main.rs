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

    let bytes: Vec<u8> = (0..=img.get_width() / 6)
        .flat_map(|state_index| {
            (state_index * 6..state_index * 6 + 5).rev()
                .flat_map(|width_index| {
                    (0..8)
                        .map(move |height_index| (width_index, height_index))
                        .flat_map(|(x, y)| {
                            let pixel = img.get_pixel(x, y);
                            [pixel.r, pixel.g, pixel.b].iter().cloned().collect::<Vec<_>>().into_iter()
                        })
                })
        })
        .collect();

    print!("{{");
    for (i, rgb) in bytes.iter().enumerate() {
        print!("{:#04X}", rgb);
        if i != bytes.len() - 1 {
            print!(",");
        }
    }
    print!("}}");
    stdout().flush().unwrap();
}
