import axios from "axios";
import { CONFIG } from "../constants/config";

export const api = axios.create({
    baseURL: CONFIG.THINGSBOARD_URL,
    timeout: 5000,
});