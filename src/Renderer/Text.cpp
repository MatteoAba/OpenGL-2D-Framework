#include "Text.h"

Text::Text(const std::string& fontPath, Application* owner)
	: m_Owner(owner)
{
    // projection matrix setup
    m_Projection = glm::ortho(0.0f, static_cast<float>(owner->GetWindow()->GetViewportWidth()), 0.0f, static_cast<float>(owner->GetWindow()->GetViewportHeight()), -1.0f, 1.0f);

    // blending setup
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // load FreeType library
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        LOG_ERROR("Failed to load FreeType Library");

    // load the font
    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
        LOG_ERROR("Failed to load font from {}", fontPath.c_str());

    // setup characters height to 48px and automatic width (0)
    FT_Set_Pixel_Sizes(face, 0, 48);

    // setup OpenGL to use one byte for color instead of four
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // load all the 128 ASHII characters in the map
    for (unsigned char c = 0; c < 128; c++) {
        // load the character's glyph (it contains all the info info)
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        // generate the texture for the character (GL_RED because only the first byte of the color is used)
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        // setup texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // create the character struct and insert it in the map 
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        m_Characters.insert(std::pair<char, Character>(c, character));
    }

    // deallocate freetype library (the caracters are saved in the map)
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // create buffers
    m_VAO = new VertexArray();
    m_VBO = new VertexBuffer(6 * 4 * sizeof(float));
    VertexLayout layout;
    layout.AddVertexGroup({ DataType::CGL_FLOAT, 4, false });
    m_VAO->AddBuffer(layout, *m_VBO);

    // load the shader
    m_Shader = new Shader("Text", "assets/Shader/Text.vert", "assets/Shader/Text.frag");
    m_Shader->Bind();
    m_Shader->SetMat4("u_Projection", m_Projection);
}

Text::~Text()
{
    delete m_Shader;
    delete m_VBO;
    delete m_VAO;
}


void Text::RenderText(std::string text, glm::vec2 position, float scale, glm::vec3 color, Framebuffer* FBO)
{
    // prepare shader, VAO and Texture Slot for the rendering
    m_Shader->Bind();
    m_Shader->SetFloat3("textColor", { color.x, color.y, color.z });
    glActiveTexture(GL_TEXTURE0);
    m_VAO->Bind();

    // analyze every charater of the string
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        // extract the character from the map
        Character ch = m_Characters[*c];
        float xpos = position.x + ch.Bearing.x * scale;
        float ypos = position.y - (ch.Size.y - ch.Bearing.y) * scale;        // lower the under the row if needed
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        // create the vertices
        float vertices[6][4] = {
            { xpos, ypos + h, 0.0f, 0.0f },
            { xpos, ypos, 0.0f, 1.0f },
            { xpos + w, ypos, 1.0f, 1.0f },
            { xpos, ypos + h, 0.0f, 0.0f },
            { xpos + w, ypos, 1.0f, 1.0f },
            { xpos + w, ypos + h, 1.0f, 0.0f }
        };

        // bind the texture
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // draw the character in a quad
        m_VAO->Bind();
        m_VBO->Bind();
        m_VBO->SubmitData(vertices, sizeof(vertices));
        Renderer::DrawQuad(m_VAO, m_Shader, FBO);

        // move to the next glyph (1/64 pixel ahead)
        position.x += (ch.Advance >> 6) * scale;                               // scale * (adv / 64)
    }

    // unbind of VBO and Texture
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
