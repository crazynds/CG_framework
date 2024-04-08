from PIL import Image
import os

def resize_image(png_file):
    try:
        # Abrir o arquivo PNG
        img = Image.open(png_file)
        
        # Redimensionar para um terço da proporção original
        new_width = img.width // 4
        new_height = img.height // 4
        resized_img = img.resize((new_width, new_height))
        
        return resized_img
    except Exception as e:
        print(f"Erro ao redimensionar {png_file}: {e}")
        return None

def convert_to_monochrome_bmp(png_file, output_folder):
    filename = os.path.basename(png_file)
    try:
        # Redimensionar a imagem
        resized_img = resize_image(png_file)
        if resized_img is not None:
            # Converter para modo Monochrome
            monochrome_img = resized_img.convert('1')
            
            # Criar o nome do arquivo de saída
            output_file = os.path.join(output_folder, os.path.splitext(filename)[0] + '.bmp')
            
            # Salvar a imagem como bitmap
            monochrome_img.save(output_file)
            
            print(f"{filename} convertido com sucesso para Monochrome Bitmap.")
    except Exception as e:
        print(f"Erro ao converter {png_file}: {e}")

def batch_convert_to_monochrome_bmp(input_folder, output_folder):
    # Verificar se a pasta de saída existe, se não, criar
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
    
    # Listar todos os arquivos na pasta de entrada
    files = os.listdir(input_folder)
    
    # Filtrar apenas os arquivos PNG
    png_files = [file for file in files if file.lower().endswith('.png')]
    
    # Converter cada arquivo PNG para Monochrome Bitmap
    for png_file in png_files:
        png_path = os.path.join(input_folder, png_file)
        convert_to_monochrome_bmp(png_path, output_folder)

# Definir a pasta de entrada e a pasta de saída
input_folder = './image_sequence'
output_folder = './image_sequence2'

# Chamar a função para converter em lote
batch_convert_to_monochrome_bmp(input_folder, output_folder)