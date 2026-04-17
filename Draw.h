void DrawESP() {
    if (!espConfig.Enable) return;

    ImDrawList* draw = ImGui::GetBackgroundDrawList();

    for (int i = 0; i < Response.count; i++) {
        Player& p = Response.players[i];
        if (p.position.z <= 0) continue;

        std::string name = GetNameActors(p.configID);
        ImVec2 pos = ImVec2(p.position.x, p.position.y);

        if (espConfig.Line && p.isEnemy) 
            draw->AddLine(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y), pos, IM_COL32(255, 0, 0, 255), 2.0f);

        if (espConfig.Box) 
            ESP::drawCornerBox(pos.x - 30, pos.y - 80, 60, 120, 2.0f, p.isEnemy ? IM_COL32(255,0,0,255) : IM_COL32(0,255,0,255));

        if (espConfig.HP) 
            DrawVerticalHealthBar(draw, p.actorHp, p.actorHpTotal, ImVec2(pos.x, pos.y - 90), 8, 80);

        if (espConfig.PlayerInfo) 
            ImGui::GetBackgroundDrawList()->AddText(NULL, 18.0f, ImVec2(pos.x - 20, pos.y - 110), IM_COL32(255,255,255,255), name.c_str());

        if (espConfig.Cooldown && p.skill1CD > 0) {
            float cd[5] = {p.skill1CD, p.skill2CD, p.skill3CD, p.skill4CD, p.skill5CD};
            DrawCooldown(draw, cd, ImVec2(pos.x - 40, pos.y + 40), ImVec2(pos.x + 40, pos.y + 60), 20);
        }

        if (espConfig.HeroImage && HeadPortrait[p.configID].texture) {
            ImGui::GetBackgroundDrawList()->AddImage(HeadPortrait[p.configID].texture, ImVec2(pos.x - 25, pos.y - 140), ImVec2(pos.x + 25, pos.y - 90));
        }
    }

    for (int i = 0; i < Response.c_mon; i++) {
        Monster& m = Response.sMonster[i];
        if (m.position.z <= 0) continue;

        std::string name = GetNameMonsters(m.configID);
        ImVec2 pos = ImVec2(m.position.x, m.position.y);

        if (espConfig.Line2) 
            draw->AddLine(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), pos, IM_COL32(255, 165, 0, 255), 1.5f);

        if (espConfig.HP2) 
            DrawVerticalHealthBar(draw, m.Hp, m.HpTotal, ImVec2(pos.x, pos.y - 50), 6, 60);

        if (espConfig.PlayerInfo2) 
            ImGui::GetBackgroundDrawList()->AddText(NULL, 16.0f, ImVec2(pos.x - 20, pos.y - 70), IM_COL32(255,255,100,255), name.c_str());
    }
}
